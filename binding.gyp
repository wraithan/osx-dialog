{
	"targets": [
		{
			"target_name": "dialog",
			"sources": [
				"src/dialog.cc",
				"src/dialog-worker.cc"
			],
			"include_dirs": [
				"<!(node -e \"require('nan')\")",
				"System/Library/Frameworks/CoreFoundation.Framework/Headers",
			],
			"link_settings": {
				"libraries": [
					"-framework CoreFoundation"
				]
			}
		}
	]
}
