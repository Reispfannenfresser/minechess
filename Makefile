all: linux windows web


linux: linux_release linux_debug

linux_release:
	scons platform=linux target=template_release

linux_debug:
	scons platform=linux target=template_debug debug_symbols=yes


windows: windows_release windows_debug

windows_release:
	scons platform=windows target=template_release

windows_debug:
	scons platform=windows target=template_debug debug_symbols=yes


web: web_singlethreaded web_multithreaded


web_singlethreaded: web_singlethreaded_release web_singlethreaded_debug

web_singlethreaded_release:
	scons platform=web target=template_release threads=false

web_singlethreaded_debug:
	scons platform=web target=template_debug threads=false debug_symbols=yes


web_multithreaded: web_multithreaded_release web_multithreaded_debug

web_multithreaded_release:
	scons platform=web target=template_release threads=true

web_multithreaded_debug:
	scons platform=web target=template_debug threads=true debug_symbols=yes
