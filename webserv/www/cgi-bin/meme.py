#!/opt/pyenv/versions/3.13.1/bin/python3

##!/usr/bin/env python3

import cgi
import sys
import os
import textwrap
from PIL import Image, ImageDraw, ImageFont

def draw_multiline_centered(draw, text, box_x, box_y, box_w, box_h, font, fill="white"):
	# wrap lines to fit box
	lines = textwrap.wrap(text, width=23)

	# Measure total text height
	line_heights = []
	for line in lines:
		h = font.size
		line_heights.append(h)
	total_h = sum(line_heights)

	# Start y so that the block is vertically centered in the box
	y = box_y + (box_h - total_h) / 2

	for line, h in zip(lines, line_heights):
		w = draw.textlength(line, font=font)
		x = box_x + (box_w - w) / 2  # horizontally center in box
		draw.text((x, y), line, font=font, fill=fill)
		y += h


env = cgi.FieldStorage()
jim_text = env.getvalue('jimtext', 'JIM')
dwight_text = env.getvalue('dwighttext', 'DWIGHT')
michael_text = env.getvalue('michaeltext', 'MICHAEL')

img_path = os.path.join(os.path.dirname(__file__), "assets", "officememe.png")
img = Image.open(img_path)

draw = ImageDraw.Draw(img)

font_path = os.path.join(os.path.dirname(__file__), "assets", "open-sans", "static", "OpenSans-SemiBold.ttf")
font = ImageFont.truetype(font_path, 14)

j_box = (50, 130, 130, 200)
d_box = (260, 600, 220, 170)
m_box = (420, 200, 190, 170)

draw_multiline_centered(draw, jim_text, *j_box, font=font)
draw_multiline_centered(draw, dwight_text, *d_box, font=font)
draw_multiline_centered(draw, michael_text, *m_box, font=font)

img.save(sys.stdout.buffer, 'PNG')