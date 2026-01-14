local glfw = require("moonglfw")
local nk = require("moonnuklear")
local backend = require("deps/lua/nuklear/backend")
local pp = require("deps/lua/pprint/pprint")

local op = 'easy'
local value = 0.6
local window_flags = nk.WINDOW_NO_SCROLLBAR

win_w = 300
win_h = 300

local function main_window(ctx)
  if nk.window_begin(ctx, "Show", {0, 0, win_w, win_h}, window_flags) then
    local region = nk.window_get_content_region_size(ctx)
    local reg_w = region[1]
    local reg_h = region[2]

    nk.style_set_vec2(ctx, "window.padding", {0,0})
    nk.style_set_vec2(ctx, "window.spacing", {0,0})
    nk.style_set_vec2(ctx, "window.group_padding", {0,0})

    local spacing = nk.style_get_vec2(ctx, "window.spacing")
    local hor_spacing = spacing[1]
    local ver_spacing = spacing[2]

    nk.layout_row_dynamic(ctx, 30, 5)
    nk.button(ctx, nil, "one")
    nk.button(ctx, nil, "two")
    nk.button(ctx, nil, "three")
    nk.button(ctx, nil, "four")
    nk.button(ctx, nil, "five")

    nk.layout_row_dynamic(ctx, reg_h - 45, 1)
    if nk.group_begin(ctx, "files", nk.WINDOW_NO_SCROLLBAR|nk.WINDOW_BORDER) then
      nk.layout_row_dynamic(ctx, 0, 1)
      nk.button(ctx, nil, "size")
    end
    nk.group_end(ctx)

    nk.layout_row_dynamic(ctx, 0, 1)
    nk.label(ctx, "/foo/bar/baz/sepi.c", nk.TEXT_ALIGN_LEFT)
  end
  nk.window_end(ctx)
end

backend.init(win_w, win_h, "playground", true, nil)
backend.loop(main_window, {.13, .29, .53, 1}, 30)

