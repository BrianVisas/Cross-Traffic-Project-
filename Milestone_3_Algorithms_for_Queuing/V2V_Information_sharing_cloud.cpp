V2V_Information_sharing()
{
	
	while(1)
	{
		collect_information_vehicle_B();// Vehicle B collect information from on board sensors 
		if(collection=true) // true: The information is collected and ready to be sent
		{
		printf("Data collection from vehicle B is  done successfully ")
		send_info_CU_Cloud(); //Vehicle B send information to the cloud based controll unit 
		printf("Information  is sent successfully")
		
		}
		else
		 {
			printf("Information  is not sent successfully")
			run_diagnostic() // in case  the information  is not sent successfully on board main controller run a diagnoses
		}
		
		
		verify_info(); //after the information is received by cloud based control unit it is verified and processed.
	   
		if(verificatio=true) // true: verification and procession is done successfully 
		{
			printf("Information  is ready for broadcasting")
			broadcact();  //Information is distributed from cloud based controll unit to Vehicle A
			
		}
		else 
		{
			printf("Information  is not sent successfully")
			
			
			process_info_again(); // Information is not validated and is still in processing phase 
		    
	   }
	   
	   if(information_received_Vehicle_A=true)// true:The informatin is reveibed successfully by vehicle A
	   {
	   	received_confirmation();// Vehicle A send back a confirmation that information is received successfully
	   		printf("Information  is received successfully")
	   }
	   else 
	   {
	   	receive_confirmation_failure();// information is  not received successfully
	   		printf("Information  is  not received successfully")
	   }
	}
return 0;	
}

