windows-powershell-getprocess
=========


> is a NodeJS module and will help you getting all  runing processes on windows systems.

Version
----

0.0.5

Tech
====

To get things work, you will need:

* [Windows Management Framework] - used to get access to the System.Management namespace and to execute PowerShell commands
* [Python 2.7] - to build the module
* [VisualStudio] - to build the module. At least version 2010 Express.


Installation
====

```javascript
npm install windows-powershell-getprocess
```


Useage
=====


getProcess([pid])
-----

either
returns an array of running processes:
```javascript
[{ Name: 'winlogon',
    ProcessName: 'winlogon',
    Id: 480,
    FileVersion: null,
    ProductVersion: null,
    Company: null,
    Path: null,
    Description: null },
  { Name: 'WINWORD',
    ProcessName: 'WINWORD',
    Id: 2032,
    FileVersion: '15.0.4569.1504',
    ProductVersion: '15.0.4569.1504',
    Company: 'Microsoft Corporation',
    Path: 'C:\\Program Files\\Microsoft Office\\Office15\\WINWORD.EXE',
    Description: 'Microsoft Word' }]
```
or an object with process information about the specified PID:
```javascript
{ Name: 'node',
  ProcessName: 'node',
  Id: 5612,
  FileVersion: '0.10.29',
  ProductVersion: '0.10.29',
  Company: 'Joyent, Inc',
  Path: 'D:\\Program Files\\nodejs\\node.exe',
  Description: 'Evented I/O for V8 JavaScript' }
```

getCPUUsage()
-----

returns an object containing PIDs as keys and ProcessName and CPUTicks as object.
Additionally holds a "TotalTicks" property as sum of CPUTicks.

```javascript
{ '6128':
   { ProcessName: 'Foxit Reader',
     CPUticks: 360986314 },
  '6228':
   { ProcessName: 'chrome',
     CPUticks: 45552292 },
  '6592':
   { ProcessName: 'chrome',
     CPUticks: 152880980 },
  '6808':
   { ProcessName: 'chrome',
     CPUticks: 49140315 },
  '6824':
   { ProcessName: 'conhost',
     CPUticks: 9516061 },
  '7064':
   { ProcessName: 'chrome',
     CPUticks: 24024154 },
  '7144':
   { ProcessName: 'chrome',
     CPUticks: 11232072 },
  totalTicks: 42332587361 }
```

getChildProcess(pid)
-----

returns an array with PIDs of childprocesses spawned by the specified pid
```javascript
[ 7144 ]
```

License
====

MIT

[Windows Management Framework]:http://www.microsoft.com/de-de/download/details.aspx?id=40855
[Python 2.7]:https://www.python.org/download/releases/2.7.6/
[VisualStudio]:http://www.microsoft.com/visualstudio/eng/downloads#d-2010-express