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
 * ff8_data.h - raw data & setup required to interact with FF8.exe
 */

#pragma once

#include "patch.h"

// FF8 game mode definitions
static struct game_mode ff8_modes[] = {
	{FF8_MODE_0,             "MODE_0",             MODE_UNKNOWN,       true },
	{FF8_MODE_1,             "MODE_1",             MODE_UNKNOWN,       true },
	{FF8_MODE_WORLDMAP,      "MODE_WORLDMAP",      MODE_WORLDMAP,      true },
	{FF8_MODE_SWIRL,         "MODE_SWIRL",         MODE_SWIRL,         true },
	{FF8_MODE_4,             "MODE_4",             MODE_UNKNOWN,       true },
	{FF8_MODE_5,             "MODE_5",             MODE_UNKNOWN,       true },
	{FF8_MODE_MENU,          "MODE_MENU",          MODE_MENU,          true },
	{FF8_MODE_7,             "MODE_7",             MODE_UNKNOWN,       true },
	{FF8_MODE_CARDGAME,      "MODE_CARDGAME",      MODE_CARDGAME,      true },
	{FF8_MODE_9,             "MODE_9",             MODE_UNKNOWN,       true },
	{FF8_MODE_10,            "MODE_10",            MODE_UNKNOWN,       true },
	{FF8_MODE_11,            "MODE_11",            MODE_UNKNOWN,       true },
	{FF8_MODE_INTRO,         "MODE_INTRO",         MODE_INTRO,         true },
	{FF8_MODE_100,           "MODE_100",           MODE_UNKNOWN,       true },
	{FF8_MODE_BATTLE,        "MODE_BATTLE",        MODE_BATTLE,        true },
};

void ff8_set_main_loop(uint driver_mode, uint main_loop)
{
	uint i;

	for(i = 0; i < num_modes; i++) if(ff8_modes[i].driver_mode == driver_mode) ff8_modes[i].main_loop = main_loop;
}

void ff8_find_externals()
{
	uint battle_main_loop;

	ff8_externals.sub_401ED0 = version == VERSION_FF8_12_JP ? 0x402290 : 0x401ED0;
	ff8_externals.pubintro_init = get_absolute_value(ff8_externals.sub_401ED0, 0x158);
	ff8_externals.sub_467C00 = get_relative_call(ff8_externals.pubintro_init, 0xB5);
	ff8_externals.sub_468810 = get_relative_call(ff8_externals.sub_467C00, 0x59);
	ff8_externals.sub_468BD0 = get_relative_call(ff8_externals.sub_468810, 0x5B);
	common_externals.dinput_hack1 = ff8_externals.sub_468BD0 + 0x64;

	ff8_externals.pubintro_main_loop = get_absolute_value(ff8_externals.sub_401ED0, 0x180);
	ff8_externals.credits_main_loop = get_absolute_value(ff8_externals.pubintro_main_loop, 0x6D);
	ff8_externals.sub_470520 = get_absolute_value(ff8_externals.credits_main_loop, 0xE2);
	ff8_externals.sub_4A24B0 = get_absolute_value(ff8_externals.sub_470520, 0x2B);
	ff8_externals.sub_470630 = get_absolute_value(ff8_externals.sub_4A24B0, 0xE4);
	ff8_externals.main_loop = get_absolute_value(ff8_externals.sub_470630, 0x24);

	ff8_externals.swirl_main_loop = get_absolute_value(ff8_externals.main_loop, 0x4A3);

	ff8_set_main_loop(MODE_SWIRL, ff8_externals.swirl_main_loop);

	battle_main_loop = get_absolute_value(ff8_externals.swirl_main_loop, 0x50);

	ff8_set_main_loop(MODE_BATTLE, battle_main_loop);

	ff8_externals.sub_47CF60 = get_absolute_value(ff8_externals.main_loop, 0x340);
	ff8_externals.sub_47CCB0 = get_relative_call(ff8_externals.sub_47CF60, 0x1B3);
	ff8_externals.sub_534640 = get_relative_call(ff8_externals.sub_47CCB0, 0xF1);
	ff8_externals.sub_4972A0 = get_relative_call(ff8_externals.sub_534640, 0x51);
	ff8_externals.load_fonts = get_relative_call(ff8_externals.sub_4972A0, 0x16);

	ff8_externals.fonts = (font_object **)get_absolute_value(ff8_externals.load_fonts, 0x16);

	common_externals.assert_malloc = (void* (*)(uint, const char*, uint))get_relative_call(ff8_externals.load_fonts, 0x2A);

	common_externals._mode = (word *)get_absolute_value(ff8_externals.main_loop, 0x115);

	ff8_externals.pubintro_enter_main = get_absolute_value(ff8_externals.sub_401ED0, 0x16C);
	common_externals.prepare_movie = get_relative_call(ff8_externals.pubintro_enter_main, 0x12);
	common_externals.release_movie_objects = get_relative_call(common_externals.prepare_movie, 0x19E);
	common_externals.start_movie = get_relative_call(ff8_externals.pubintro_enter_main, 0x1A);

	ff8_externals.mode1_main_loop = get_absolute_value(ff8_externals.main_loop, 0x144);
	ff8_externals.sub_471F70 = get_relative_call(ff8_externals.mode1_main_loop, 0x148);
	ff8_externals.sub_4767B0 = get_relative_call(ff8_externals.sub_471F70, 0x4FE);
	common_externals.update_movie_sample = get_relative_call(ff8_externals.sub_4767B0, 0x5A5);

	ff8_externals.draw_movie_frame = get_relative_call(ff8_externals.sub_4767B0, 0xB84);
	common_externals.stop_movie = get_relative_call(common_externals.update_movie_sample, 0x3E2);

	ff8_externals.sub_529FF0 = get_relative_call(ff8_externals.sub_4767B0, 0x14E);
	common_externals.get_movie_frame = get_relative_call(ff8_externals.sub_529FF0, 0x26);

	ff8_externals.movie_object = (ff8_movie_obj *)get_absolute_value(common_externals.prepare_movie, 0xDB);

	ff8_externals.mode1_main_loop = get_absolute_value(ff8_externals.main_loop, 0x144);
	ff8_externals.sub_471F70 = get_relative_call(ff8_externals.mode1_main_loop, 0x148);
	ff8_externals.sub_4767B0 = get_relative_call(ff8_externals.sub_471F70, 0x4FE);
	common_externals.update_movie_sample = get_relative_call(ff8_externals.sub_4767B0, 0x5A5);
	common_externals.debug_print = get_relative_call(common_externals.update_movie_sample, 0x141);

	ff8_externals._load_texture = get_relative_call(ff8_externals.load_fonts, 0x197);
	ff8_externals.sub_4076B6 = get_relative_call(ff8_externals._load_texture, 0x16D);
	ff8_externals.sub_41AC34 = get_relative_call(ff8_externals.sub_4076B6, 0x46);
	ff8_externals.load_texture_data = get_relative_call(ff8_externals.sub_41AC34, 0x168);
	common_externals.load_tex_file = get_relative_call(ff8_externals.load_texture_data, 0x103);
	common_externals.create_tex_header = (tex_header* (*)())get_relative_call(common_externals.load_tex_file, 0xD);
	common_externals.assert_calloc = (void* (*)(uint, uint, const char*, uint))get_relative_call((uint)common_externals.create_tex_header, 0x15);
	common_externals.open_file = get_relative_call(common_externals.load_tex_file, 0x27);
	common_externals.read_file = get_relative_call(common_externals.load_tex_file, 0x49);
	common_externals.alloc_read_file = (void* (*)(uint, uint, struct file *))get_relative_call(common_externals.load_tex_file, 0xB3);
	common_externals.close_file = get_relative_call(common_externals.load_tex_file, 0x15B);
	common_externals.destroy_tex = (void (*)(tex_header*))get_relative_call(common_externals.load_tex_file, 0x16D);
	common_externals.destroy_tex_header = get_relative_call((uint)common_externals.destroy_tex, 0x78);
	common_externals.assert_free = (void* (*)(void*, const char*, uint))get_relative_call(common_externals.destroy_tex_header, 0x21);
	common_externals.get_game_object = (game_obj* (*)())get_relative_call((uint)common_externals.destroy_tex, 0x6);

	ff8_externals.dd_d3d_start = get_relative_call(ff8_externals.pubintro_init, 0x75);
	ff8_externals.create_d3d_gfx_driver = get_relative_call(ff8_externals.dd_d3d_start, 0x88);
	ff8_externals.d3d_init = get_absolute_value(ff8_externals.create_d3d_gfx_driver, 0x1B);
	ff8_externals.sub_40BFEB = get_absolute_value(ff8_externals.d3d_init, 0x1370);
	common_externals.create_texture_format = (struct texture_format* (*)())get_relative_call(ff8_externals.sub_40BFEB, 0x2B);

	ff8_externals.tim2tex = get_relative_call(ff8_externals.sub_41AC34, 0xFC);
	ff8_externals.sub_41BC76 = get_relative_call(ff8_externals.tim2tex, 0x72);
	common_externals.make_pixelformat = (void (*)(uint, uint, uint, uint, uint, struct texture_format*))get_relative_call(ff8_externals.sub_41BC76, 0x102);

	common_externals.add_texture_format = (void (*)(struct texture_format*, game_obj*))get_relative_call(ff8_externals.sub_40BFEB, 0xBF);

	ff8_externals.d3d_load_texture = get_absolute_value(ff8_externals.create_d3d_gfx_driver, 0x9D);
	common_externals.create_texture_set = (texture_set* (*)())get_relative_call(ff8_externals.d3d_load_texture, 0x6B);

	common_externals.create_palette_for_tex = (palette* (*)(uint, tex_header*, texture_set*))get_relative_call(ff8_externals.d3d_load_texture, 0x316);

	ff8_externals.movie_hack1 = common_externals.update_movie_sample + 0xA5;
	ff8_externals.movie_hack2 = common_externals.update_movie_sample + 0x1DF;

	ff8_externals.sub_559910 = get_relative_call(ff8_externals.swirl_main_loop, 0x1A);

	ff8_externals.swirl_sub_56D1D0 = get_relative_call(ff8_externals.sub_47CF60, 0x285);
	ff8_externals.swirl_sub_56D390 = get_relative_call(ff8_externals.swirl_sub_56D1D0, 0x2A);
	ff8_externals.swirl_texture1 = (ff8_graphics_object **)get_absolute_value(ff8_externals.swirl_sub_56D1D0, 0x1);

	ff8_externals.load_credits_image = get_relative_call(ff8_externals.credits_main_loop, 0xBF);
	ff8_externals.sub_52FE80 = get_relative_call(ff8_externals.load_credits_image, 0xA4);
	ff8_externals.sub_45D610 = get_relative_call(ff8_externals.sub_52FE80, 0x90);
	ff8_externals.sub_45D080 = get_relative_call(ff8_externals.sub_45D610, 0x5);
	ff8_externals.sub_464BD0 = get_relative_call(ff8_externals.sub_45D080, 0x208);
	ff8_externals.sub_4653B0 = get_relative_call(ff8_externals.sub_464BD0, 0x79);
	ff8_externals.sub_465720 = get_relative_call(ff8_externals.sub_464BD0, 0xAF);

	ff8_externals.sub_559E40 = get_relative_call(ff8_externals.swirl_main_loop, 0x28);
	ff8_externals.sub_559F30 = get_relative_call(ff8_externals.sub_559E40, 0xC1);

	if(NV_VERSION)
	{
		ff8_externals.nvidia_hack1 = get_absolute_value(ff8_externals.sub_559F30, 0x3E);
		ff8_externals.nvidia_hack2 = get_absolute_value(ff8_externals.sub_559F30, 0xAC);
	}

	ff8_externals.menu_viewport = (sprite_viewport *)(get_absolute_value(ff8_externals.sub_4972A0, 0x12) - 0x20);

	ff8_externals.sub_4A24B0 = get_absolute_value(ff8_externals.sub_470520, 0x2B);
	ff8_externals.sub_497380 = get_relative_call(ff8_externals.sub_4A24B0, 0xAA);
	ff8_externals.sub_4B3410 = get_relative_call(ff8_externals.sub_497380, 0xAC);
	ff8_externals.sub_4BE4D0 = get_relative_call(ff8_externals.sub_4B3410, 0x68);
	ff8_externals.sub_4BECC0 = get_relative_call(ff8_externals.sub_4BE4D0, 0x39);
	ff8_externals.menu_draw_text = get_relative_call(ff8_externals.sub_4BECC0, 0x127);
	ff8_externals.get_character_width = (uint (*)(uint))get_relative_call(ff8_externals.menu_draw_text, 0x1D0);

	ff8_externals.open_lzs_image = get_relative_call(ff8_externals.load_credits_image, 0x27);
	ff8_externals.upload_psx_vram = get_relative_call(ff8_externals.open_lzs_image, 0xB9);
	ff8_externals.psxvram_buffer = (word *)get_absolute_value(ff8_externals.upload_psx_vram, 0x34);
	ff8_externals.sub_464850 = (void (*)(uint, uint, uint, uint))get_relative_call(ff8_externals.upload_psx_vram, 0x8A);

	ff8_externals.psx_texture_pages = (struc_51 *)get_absolute_value(ff8_externals.sub_464BD0, 0x10);

	ff8_externals.read_field_data = get_relative_call(ff8_externals.sub_471F70, 0x23A);
	ff8_externals.upload_mim_file = get_relative_call(ff8_externals.read_field_data, 0x729);
	ff8_externals.field_filename = (char *)get_absolute_value(ff8_externals.read_field_data, 0xF0);

	ff8_externals.load_field_models = get_relative_call(ff8_externals.read_field_data, 0xF0F);

	ff8_externals.worldmap_enter_main = get_absolute_value(ff8_externals.main_loop, 0x2C0);
	ff8_externals.worldmap_sub_53F310 = get_relative_call(ff8_externals.worldmap_enter_main, 0xA7);
	ff8_externals.sub_545E20 = get_relative_call(ff8_externals.worldmap_sub_53F310, 0x60C);
	ff8_externals.sub_653410 = get_relative_call(ff8_externals.sub_545E20, 0x68);

	ff8_externals.wm_upload_psx_vram = get_relative_call(ff8_externals.load_field_models, 0xB72);

	ff8_externals.check_active_window = get_relative_call(ff8_externals.pubintro_main_loop, 0x4);
	ff8_externals.sub_467D10 = get_relative_call(ff8_externals.check_active_window, 0x16);

	ff8_externals.dinput_sub_468D80 = get_relative_call(ff8_externals.sub_467D10, 0x11);
	ff8_externals.dinput_sub_4692B0 = get_relative_call(ff8_externals.sub_467D10, 0x1B);

	ff8_externals.sub_469640 = get_relative_call(ff8_externals.pubintro_init, 0xD8);
	ff8_externals.sub_46DBF0 = get_relative_call(ff8_externals.sub_469640, 0x5D);
	common_externals.directsound = (IDirectSound**)get_absolute_value(ff8_externals.sub_46DBF0, 0x26);

	ff8_externals.sub_5304B0 = (void (*)())get_relative_call(common_externals.update_movie_sample, 0x3D9);

	ff8_externals.enable_framelimiter = (uint *)get_absolute_value(common_externals.stop_movie, 0x49);

	ff8_externals.byte_1CE4907 = (unsigned char *)get_absolute_value(common_externals.update_movie_sample, 0x363);
	ff8_externals.byte_1CE4901 = (unsigned char *)get_absolute_value(common_externals.update_movie_sample, 0x37D);
	ff8_externals.byte_1CE490D = (unsigned char *)get_absolute_value(common_externals.update_movie_sample, 0x3BE);

	ff8_externals.sub_45B310 = get_relative_call(ff8_externals.pubintro_init, 0x91);
	ff8_externals.sub_45B460 = get_relative_call(ff8_externals.sub_45B310, 0x0);
	ff8_externals.ssigpu_init = get_relative_call(ff8_externals.sub_45B460, 0x26);
	ff8_externals.d3dcaps = (uint *)get_absolute_value(ff8_externals.ssigpu_init, 0x6C);

	ff8_externals.worldmap_main_loop = get_absolute_value(ff8_externals.main_loop, 0x2D0);

	if(version == VERSION_FF8_12_US || version == VERSION_FF8_12_US_NV || version == VERSION_FF8_12_US_EIDOS || version == VERSION_FF8_12_US_EIDOS_NV)
	{
		ff8_externals.sub_548080 = get_relative_call(ff8_externals.worldmap_sub_53F310, 0x579);

		ff8_externals.sub_53FAC0 = get_relative_call(ff8_externals.worldmap_main_loop, 0x134);

		ff8_externals.sub_549E80 = get_relative_call(ff8_externals.sub_53FAC0, 0x1D5);
		ff8_externals.sub_53BB90 = get_relative_call(ff8_externals.sub_53FAC0, 0x2D4);
		ff8_externals.sub_53C750 = get_relative_call(ff8_externals.sub_53FAC0, 0x2DB);
		ff8_externals.sub_54FDA0 = get_relative_call(ff8_externals.sub_53FAC0, 0x375);
		ff8_externals.sub_54D7E0 = get_relative_call(ff8_externals.sub_53FAC0, 0x3C2);
		ff8_externals.sub_544630 = get_relative_call(ff8_externals.sub_53FAC0, 0x3D2);
		ff8_externals.sub_545EA0 = get_relative_call(ff8_externals.sub_53FAC0, 0x4BF);

		ff8_externals.sub_545F10 = get_relative_call(ff8_externals.sub_545EA0, 0x20);

		ff8_externals.sub_546100 = get_relative_call(ff8_externals.sub_545F10, 0x58);
	}
	else
	{
		ff8_externals.sub_548080 = get_relative_call(ff8_externals.worldmap_sub_53F310, 0x5A2);

		ff8_externals.sub_53FAC0 = get_relative_call(ff8_externals.worldmap_main_loop, 0x137);

		ff8_externals.sub_549E80 = get_relative_call(ff8_externals.sub_53FAC0, 0x1D6);
		ff8_externals.sub_53BB90 = get_relative_call(ff8_externals.sub_53FAC0, 0x2D5);
		ff8_externals.sub_53C750 = get_relative_call(ff8_externals.sub_53FAC0, 0x2DC);
		ff8_externals.sub_54FDA0 = get_relative_call(ff8_externals.sub_53FAC0, 0x376);
		ff8_externals.sub_54D7E0 = get_relative_call(ff8_externals.sub_53FAC0, 0x3C4);
		ff8_externals.sub_544630 = get_relative_call(ff8_externals.sub_53FAC0, 0x3D5);
		ff8_externals.sub_545EA0 = get_relative_call(ff8_externals.sub_53FAC0, 0x4C1);

		ff8_externals.sub_545F10 = get_relative_call(ff8_externals.sub_545EA0, 0x1C);

		ff8_externals.sub_546100 = get_relative_call(ff8_externals.sub_545F10, 0x54);

		ff8_externals.sub_54A0D0 = 0x54A0D0;
	}

	switch (version)
	{
	case VERSION_FF8_12_US:
		ff8_externals.uvWorldMapFix = 0x553B10;
		break;
	case VERSION_FF8_12_US_NV:
		ff8_externals.requiredDisk = 0xB8EE90;
		ff8_externals.uvWorldMapFix = 0x553CC0;
		break;
	case VERSION_FF8_12_US_EIDOS:
		ff8_externals.uvWorldMapFix = 0x553A90;
		break;
	case VERSION_FF8_12_US_EIDOS_NV:
		ff8_externals.uvWorldMapFix = 0x553C40;
		break;
	case VERSION_FF8_12_FR:
		ff8_externals.uvWorldMapFix = 0x553EAF;
		break;
	case VERSION_FF8_12_FR_NV:
		ff8_externals.requiredDisk = 0xB8EDB8;
		ff8_externals.uvWorldMapFix = 0x55405F;
		break;
	case VERSION_FF8_12_DE:
		ff8_externals.uvWorldMapFix = 0x0;
		ff8_externals.uvWorldMapFix = 0x553FAF;
		break;
	case VERSION_FF8_12_DE_NV:
		ff8_externals.requiredDisk = 0xB8EDC0;
		ff8_externals.uvWorldMapFix = 0x55415F;
		break;
	case VERSION_FF8_12_SP:
		ff8_externals.uvWorldMapFix = 0x553F9F;
		break;
	case VERSION_FF8_12_SP_NV:
		ff8_externals.requiredDisk = 0xB8EDC0;
		ff8_externals.uvWorldMapFix = 0x55414F;
		break;
	case VERSION_FF8_12_IT:
		ff8_externals.uvWorldMapFix = 0x553F2F;
		break;
	case VERSION_FF8_12_IT_NV:
		ff8_externals.requiredDisk = 0xB8EDB8;
		ff8_externals.uvWorldMapFix = 0x5540DF;
		break;
	case VERSION_FF8_12_JP:
		ff8_externals.requiredDisk = 0xD92BB0;
		break;
	}
}

void ff8_data()
{
	num_modes = sizeof(ff8_modes) / sizeof(ff8_modes[0]);

	ff8_find_externals();

	memcpy(modes, ff8_modes, sizeof(ff8_modes));

	text_colors[TEXTCOLOR_GRAY] = 0x08;
	text_colors[TEXTCOLOR_BLUE] = 0x01;
	text_colors[TEXTCOLOR_RED] = 0x04;
	text_colors[TEXTCOLOR_PINK] = 0x05;
	text_colors[TEXTCOLOR_GREEN] = 0x02;
	text_colors[TEXTCOLOR_LIGHT_BLUE] = 0x09;
	text_colors[TEXTCOLOR_YELLOW] = 0x0E;
	text_colors[TEXTCOLOR_WHITE] = 0x0F;
}
