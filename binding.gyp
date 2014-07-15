{
  "targets": [
    {
      "target_name": "PowerShell",
	  'conditions': [
          ['OS=="win"', {
			"sources": [ "src/powerShell.cpp" ],
            'msvs_settings': {
              'VCCLCompilerTool': {                  
                'RuntimeTypeInfo':'true',
                'RuntimeLibrary':'MultiThreadedDLL'
              }
            },
            'msbuild_settings': {
              'ClCompile': {
                'ExceptionHandling': 'Async',
                'CompileAsManaged':'true'
              } 
            },
			'libraries' : ['advapi32.lib']
          }]
      ],
	  "include_dirs": ["src/","dll/"],
    }
  ]
}