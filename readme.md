# Collections

## Core
'''
├── src\
│   ├── controller
│   │   ├── **/*.css
│   ├── views
│   ├── model
│   ├── index.js
├── public
│   ├── css
│   │   ├── **/*.css
│   ├── images
│   ├── js
│   ├── index.html
├── dist (or build
├── node_modules
├── package.json
├── package-lock.json 
└── .gitignore
'''


## Libraries

namespaces
Memory
Quality
Log

5. verify and commit

change test time reportinh to ms instrad of ns
move log to libraries
rename libraries to core
6. all mgr derive from base mgr
7. add destructor in mgr class and call close all
8. prevent repeated dynamic cast calls every time you call one of the apis !!!! you could do mylist.LIST_PUSH,
return object from init, to get around scope issue, use get_id()