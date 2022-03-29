# MIP 2
"multum in parvo"  
(much in little)

#### plugins, wrappers, abstractions, simplifications, helpers, ..


v0.0.0  
not production-ready, in progress, prototype, proof of concept, pre-, changes are frequent, code will break..

---

- os: linux (maybe windows)
- plugin: clap (with wrappers for exe, vst2, vst3, [maybe ladspa, lv2])
- host: clap, ladspa (maybe other formats)
- gui: xcb (optionally with cairo on top)
- io: jack audio/midi (standalone executable)
- other: too much to list..

---

pro/con:

- headers-only
- lock-free
- un-modern
- un-cool c++
- minimal setup
- minimal dependencies
- permissive license
- (semi-) modular


---

priorities:

- make it work
- make it right
- make it fast, small, stable, bugfree, pretty, etc..

---

![sa_clap](doc/screenshots/sa_clap.png)