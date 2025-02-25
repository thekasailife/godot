/*************************************************************************/
/*  check_box.cpp                                                        */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                      https://godotengine.org                          */
/*************************************************************************/
/* Copyright (c) 2007-2021 Juan Linietsky, Ariel Manzur.                 */
/* Copyright (c) 2014-2021 Godot Engine contributors (cf. AUTHORS.md).   */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#include "check_box.h"

#include "servers/rendering_server.h"

Size2 CheckBox::get_icon_size() const {
	Ref<Texture2D> checked = Control::get_theme_icon(SNAME("checked"));
	Ref<Texture2D> unchecked = Control::get_theme_icon(SNAME("unchecked"));
	Ref<Texture2D> radio_checked = Control::get_theme_icon(SNAME("radio_checked"));
	Ref<Texture2D> radio_unchecked = Control::get_theme_icon(SNAME("radio_unchecked"));
	Ref<Texture2D> checked_disabled = Control::get_theme_icon(SNAME("checked_disabled"));
	Ref<Texture2D> unchecked_disabled = Control::get_theme_icon(SNAME("unchecked_disabled"));
	Ref<Texture2D> radio_checked_disabled = Control::get_theme_icon(SNAME("radio_checked_disabled"));
	Ref<Texture2D> radio_unchecked_disabled = Control::get_theme_icon(SNAME("radio_unchecked_disabled"));

	Size2 tex_size = Size2(0, 0);
	if (!checked.is_null()) {
		tex_size = Size2(checked->get_width(), checked->get_height());
	}
	if (!unchecked.is_null()) {
		tex_size = Size2(MAX(tex_size.width, unchecked->get_width()), MAX(tex_size.height, unchecked->get_height()));
	}
	if (!radio_checked.is_null()) {
		tex_size = Size2(MAX(tex_size.width, radio_checked->get_width()), MAX(tex_size.height, radio_checked->get_height()));
	}
	if (!radio_unchecked.is_null()) {
		tex_size = Size2(MAX(tex_size.width, radio_unchecked->get_width()), MAX(tex_size.height, radio_unchecked->get_height()));
	}
	if (!checked_disabled.is_null()) {
		tex_size = Size2(MAX(tex_size.width, checked_disabled->get_width()), MAX(tex_size.height, checked_disabled->get_height()));
	}
	if (!unchecked_disabled.is_null()) {
		tex_size = Size2(MAX(tex_size.width, unchecked_disabled->get_width()), MAX(tex_size.height, unchecked_disabled->get_height()));
	}
	if (!radio_checked_disabled.is_null()) {
		tex_size = Size2(MAX(tex_size.width, radio_checked_disabled->get_width()), MAX(tex_size.height, radio_checked_disabled->get_height()));
	}
	if (!radio_unchecked_disabled.is_null()) {
		tex_size = Size2(MAX(tex_size.width, radio_unchecked_disabled->get_width()), MAX(tex_size.height, radio_unchecked_disabled->get_height()));
	}
	return tex_size;
}

Size2 CheckBox::get_minimum_size() const {
	Size2 minsize = Button::get_minimum_size();
	Size2 tex_size = get_icon_size();
	minsize.width += tex_size.width;
	if (get_text().length() > 0) {
		minsize.width += get_theme_constant(SNAME("hseparation"));
	}
	Ref<StyleBox> sb = get_theme_stylebox(SNAME("normal"));
	minsize.height = MAX(minsize.height, tex_size.height + sb->get_margin(SIDE_TOP) + sb->get_margin(SIDE_BOTTOM));

	return minsize;
}

void CheckBox::_notification(int p_what) {
	if ((p_what == NOTIFICATION_THEME_CHANGED) || (p_what == NOTIFICATION_LAYOUT_DIRECTION_CHANGED || (p_what == NOTIFICATION_TRANSLATION_CHANGED))) {
		if (is_layout_rtl()) {
			_set_internal_margin(SIDE_LEFT, 0.f);
			_set_internal_margin(SIDE_RIGHT, get_icon_size().width);
		} else {
			_set_internal_margin(SIDE_LEFT, get_icon_size().width);
			_set_internal_margin(SIDE_RIGHT, 0.f);
		}
	} else if (p_what == NOTIFICATION_DRAW) {
		RID ci = get_canvas_item();

		Ref<Texture2D> on = Control::get_theme_icon(vformat("%s%s", is_radio() ? "radio_checked" : "checked", is_disabled() ? "_disabled" : ""));
		Ref<Texture2D> off = Control::get_theme_icon(vformat("%s%s", is_radio() ? "radio_unchecked" : "unchecked", is_disabled() ? "_disabled" : ""));
		Ref<StyleBox> sb = get_theme_stylebox(SNAME("normal"));

		Vector2 ofs;
		if (is_layout_rtl()) {
			ofs.x = get_size().x - sb->get_margin(SIDE_RIGHT) - get_icon_size().width;
		} else {
			ofs.x = sb->get_margin(SIDE_LEFT);
		}
		ofs.y = int((get_size().height - get_icon_size().height) / 2) + get_theme_constant(SNAME("check_vadjust"));

		if (is_pressed()) {
			on->draw(ci, ofs);
		} else {
			off->draw(ci, ofs);
		}
	}
}

bool CheckBox::is_radio() {
	return get_button_group().is_valid();
}

CheckBox::CheckBox(const String &p_text) :
		Button(p_text) {
	set_toggle_mode(true);

	set_text_alignment(HORIZONTAL_ALIGNMENT_LEFT);

	if (is_layout_rtl()) {
		_set_internal_margin(SIDE_RIGHT, get_icon_size().width);
	} else {
		_set_internal_margin(SIDE_LEFT, get_icon_size().width);
	}
}

CheckBox::~CheckBox() {
}
