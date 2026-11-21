#!/usr/bin/env python3
"""Turn web/index.html into wallball_web/page.h (the page as a C string).

Run this after every change to web/index.html:
    python3 tools/make_page.py
"""
import pathlib

root = pathlib.Path(__file__).resolve().parent.parent
html = (root / "web" / "index.html").read_text(encoding="utf-8")
assert ')rawliteral"' not in html, "index.html must not contain the text )rawliteral\""

out = ("// Web page served at http://192.168.4.1\n"
       "// Generated from web/index.html by tools/make_page.py - do not edit by hand.\n"
       "#pragma once\n\n"
       'const char INDEX_HTML[] = R"rawliteral(' + html + ')rawliteral";\n')
(root / "wallball_web" / "page.h").write_text(out, encoding="utf-8")
print("wrote wallball_web/page.h (%d bytes)" % len(out.encode("utf-8")))
