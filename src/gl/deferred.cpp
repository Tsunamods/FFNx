/* 
 * FFNx - Complete OpenGL replacement of the Direct3D renderer used in 
 * the original ports of Final Fantasy VII and Final Fantasy VIII for the PC.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * gl/deferred.c - implements triangle re-ordering to achieve correct blending
 */

#include "../renderer.h"

#include "../types.h"
#include "../gl.h"
#include "../macro.h"
#include "../log.h"

uint nodefer = false;

#define DEFERRED_MAX 1024

struct deferred_draw *deferred_draws;
uint num_deferred;

// re-order and save a draw call for later processing
uint gl_defer_draw(uint primitivetype, uint vertextype, struct nvertex *vertices, uint vertexcount, word *indices, uint count, uint clip, uint mipmap)
{
	uint tri;
	uint mode = getmode_cached()->driver_mode;
	uint *tri_deferred;
	float *tri_z;
	uint defer_index = 0;

	if (trace_all) trace("gl_defer_draw: call with primitivetype: %u - vertextype: %u - vertexcount: %u - count: %u - clip: %d - mipmap: %d\n", primitivetype, vertextype, vertexcount, count, clip, mipmap);

	if(!deferred_draws) deferred_draws = (deferred_draw*)driver_calloc(sizeof(*deferred_draws), DEFERRED_MAX);

	// global disable
	if (nodefer) {
		if (trace_all) trace("gl_defer_draw: nodefer true\n");
		return false;
	}

	// output will not be consistent if depth testing is disabled, this call
	// cannot be re-ordered
	if(!current_state.depthtest)
	{
		if (trace_all) trace("gl_defer_draw: depthtest false\n");
		return false;
	}

	// framebuffer textures should not be re-ordered
	if(current_state.fb_texture)
	{
		if (trace_all) trace("gl_defer_draw: fb_texture true\n");
		return false;
	}

	if(current_state.blend_mode != BLEND_NONE)
	{
		if (trace_all) trace("gl_defer_draw: blend_mode != BLEND_NONE - blend_mode: %u\n", current_state.blend_mode);
		if(current_state.blend_mode != BLEND_AVG)
		{
			if (trace_all) trace("gl_defer_draw: blend_mode != BLEND_AVG - blend_mode: %u\n", current_state.blend_mode);
			// be conservative with non-standard blending modes
			if (mode != MODE_MENU && mode != MODE_BATTLE) {
				if (trace_all) trace("gl_defer_draw: mode != MODE_MENU && mode != MODE_BATTLE - mode: %u\n", mode);
				return false;
			}
		}
	}
	else
	{
		if (trace_all) trace("gl_defer_draw: fancy_transparency brought us here\n");
		// fancy_transparency brought us here, blending was not enabled for
		// this texture originally

		if (!current_state.texture_set)
		{
			if (trace_all) trace("gl_defer_draw: texture_set false\n");
			return false;
		} 
		else
		{
			VOBJ(texture_set, texture_set, current_state.texture_set);
			VOBJ(tex_header, tex_header, VREF(texture_set, tex_header));

			if (trace_all) trace("gl_defer_draw: texture_set true for texture %s%d\n", VREF(tex_header, file.pc_name), VREF(tex_header, palette_index));

			// texture format does not support alpha, re-order is not necessary
			if (!VREF(texture_set, ogl.external) && VREF(tex_header, tex_format.alpha_bits) < 2) {
				if (trace_all) trace("gl_defer_draw: texture format does not support alpha, re-order is not necessary\n");
				return false;
			}
		}
	}

	// quads are used for some GUI elements, we do not need to re-order these
	if (primitivetype != RendererPrimitiveType::PT_TRIANGLES) {
		if (trace_all) trace("gl_defer_draw: primitivetype != TRIANGLES\n");
		return false;
	}

	if(num_deferred + count / 3 > DEFERRED_MAX)
	{
		if (trace_all) trace("gl_defer_draw: deferred draw queue overflow - num_deferred: %u - count: %u - DEFERRED_MAX: %u\n", num_deferred, count, DEFERRED_MAX);
		return false;
	}

	tri_deferred = (uint*)driver_calloc(sizeof(*tri_deferred), count / 3);
	tri_z = (float*)driver_calloc(sizeof(*tri_z), count / 3);

	// calculate screen space average Z coordinate for each triangle
	for(tri = 0; tri < count / 3; tri++)
	{
		uint i;

		for(i = 0; i < 3; i++)
		{
			if(vertextype == TLVERTEX) tri_z[tri] += vertices[indices[tri * 3 + i]]._.z;
			else
			{
				struct point4d world;
				struct point4d proj;
				struct point4d view;
				transform_point_w(&current_state.world_matrix, &vertices[indices[tri * 3 + i]]._, &world);
				transform_point4d(&current_state.d3dprojection_matrix, &world, &proj);
				transform_point4d(&d3dviewport_matrix, &proj, &view);
				tri_z[tri] += view.z / view.w;
			}
		}

		tri_z[tri] /= 3.0f;
	}

	// arrange triangles into layers based on Z coordinates calculated above
	// each layer will be drawn separately
	while(defer_index < count / 3)
	{
		float z = tri_z[defer_index];
		uint tri_num = 0;
		uint defer = num_deferred;
		uint vert_index = 0;

		for(tri = 0; tri < count / 3; tri++) if(tri_z[tri] == z) tri_num++;

		deferred_draws[defer].count = tri_num * 3;
		deferred_draws[defer].clip = clip;
		deferred_draws[defer].mipmap = mipmap;
		deferred_draws[defer].primitivetype = primitivetype;
		deferred_draws[defer].vertextype = vertextype;
		deferred_draws[defer].vertexcount = tri_num * 3;
		deferred_draws[defer].indices = (word*)driver_malloc(sizeof(*indices) * tri_num * 3);
		deferred_draws[defer].vertices = (nvertex*)driver_malloc(sizeof(*vertices) * tri_num * 3);
		gl_save_state(&deferred_draws[defer].state);
		deferred_draws[defer].drawn = false;
		deferred_draws[defer].z = z;

		for(tri = 0; tri < count / 3 && vert_index < tri_num * 3; tri++)
		{
			if(tri_z[tri] == z)
			{
				memcpy(&deferred_draws[defer].vertices[vert_index + 0], &vertices[indices[tri * 3 + 0]], sizeof(*vertices));
				memcpy(&deferred_draws[defer].vertices[vert_index + 1], &vertices[indices[tri * 3 + 1]], sizeof(*vertices));
				memcpy(&deferred_draws[defer].vertices[vert_index + 2], &vertices[indices[tri * 3 + 2]], sizeof(*vertices));
				deferred_draws[defer].indices[vert_index + 0] = vert_index + 0;
				deferred_draws[defer].indices[vert_index + 1] = vert_index + 1;
				deferred_draws[defer].indices[vert_index + 2] = vert_index + 2;

				vert_index += 3;

				tri_deferred[tri] = true;
			}
		}

		if(vert_index < tri_num * 3) error("deferred draw z mismatch\n");

		num_deferred++;

		while(defer_index < count / 3 && tri_deferred[defer_index]) defer_index++;
	}

	driver_free(tri_deferred);
	driver_free(tri_z);

	if (trace_all) trace("gl_defer_draw: return true\n");

	return true;
}

// draw all the layers we've accumulated in the correct order and reset queue
void gl_draw_deferred()
{
	struct driver_state saved_state;

	if (num_deferred == 0) {
		if (trace_all) trace("gl_draw_deferred: num_deferred == 0\n");
		return;
	}

	gl_save_state(&saved_state);

	nodefer = true;

	stats.deferred += num_deferred;

	while(true)
	{
		uint i;
		double z = -1.0;
		uint next = -1;

		for(i = 0; i < num_deferred; i++)
		{
			if(deferred_draws[i].z > z && !deferred_draws[i].drawn)
			{
				next = i;
				z = deferred_draws[i].z;
			}
		}

		if(next == -1) break;

		gl_load_state(&deferred_draws[next].state);
		internal_set_renderstate(V_DEPTHTEST, 1, 0);
		internal_set_renderstate(V_DEPTHMASK, 1, 0);

		gl_draw_indexed_primitive(deferred_draws[next].primitivetype, 
								  deferred_draws[next].vertextype, 
								  deferred_draws[next].vertices, 
								  deferred_draws[next].vertexcount, 
								  deferred_draws[next].indices, 
								  deferred_draws[next].count, 
								  0, 
								  deferred_draws[next].clip, 
								  deferred_draws[next].mipmap
								  );

		driver_free(deferred_draws[next].vertices);
		driver_free(deferred_draws[next].indices);
		deferred_draws[next].drawn = true;
	}

	num_deferred = 0;

	nodefer = false;

	gl_load_state(&saved_state);
}

// a texture is being unloaded, invalidate any pending draw calls associated
// with it and perform the necessary cleanup
void gl_check_deferred(struct texture_set *texture_set)
{
	uint i;

	for(i = 0; i < num_deferred; i++)
	{
		if(deferred_draws[i].state.texture_set == texture_set)
		{
			driver_free(deferred_draws[i].vertices);
			driver_free(deferred_draws[i].indices);
			deferred_draws[i].drawn = true;
		}
	}
}

void gl_cleanup_deferred()
{
	driver_free(deferred_draws);
}