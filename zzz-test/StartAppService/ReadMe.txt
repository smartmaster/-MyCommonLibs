
1. service management

	a. create service
		this.exe -create
	
	b. start service
		this.exe -start
		(or use services.msc)
	
	c. stop service
		this.exe -stop
		(or use services.msc)
		
	d. pulse service
		this.exe -pulse
		(or use services.msc)
	
	e. change startup type
		this.exe -change [a|m|d]
		(or use services.msc)
	
	f. delete service
		this.exe -delete
		
	
		
		
2. service start any app
	
	1) create service using command line or manually creating service registry key
	2) edit CAStartAppService.ini, please refer to the sample INI
	3) start service, then the app will be started by service


