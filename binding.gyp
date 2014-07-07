{
  "targets": [
    {
      "target_name": "PowerShell",
      "sources": [ "src/powerShell.cpp" ],
	  'conditions': [
          ['OS=="win"', {
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