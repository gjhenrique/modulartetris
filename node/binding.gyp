{
  "variables": {
      "tetris_root": "<(module_root_dir)/.."
  },
  "targets": [
    {
      "target_name": "modular-tetris",
      "include_dirs": ["<@(tetris_root)/tetris"],
      "sources": ["modulartetris_wrap.cpp"],
      "link_settings": {
          "libraries": ["-ltetris"],
          "ldflags": ["-L<@(tetris_root)", "-Wl,-rpath,<@(tetris_root)"]
      }
    }
  ]
}
