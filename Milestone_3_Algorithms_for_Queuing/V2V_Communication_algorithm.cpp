
V2V_Communication()
{
	get_position();  //collect information for position from the vehicle sensors 
	start_communication(); // Start wifi communication between vehicles 
	
	if(data=true) //True : data collection from sensors is done successfully
	{
		while(1)
		{
			
			send_data_vehicle_controller();
			if(collision_prediction=true) //true: collision detection system has processed data and potention collision is present
			{
				collision_warning();
				if(collision_avoidance=true) //true:collision avoided successfully
				printf("Collision is avoided successfully")
				else 
				printf("Collision is still posible")
			}
			else
			{
				printf("No Collision is detected")
				safe_drive(); //vehicle can continue a normal ride as it was predicted 
			}
		}
	}
	
	else
	{
		printf("Data collection from sensors is not done successfully")
		sensor_failure (); //This funcion is called in case a failure to collect data from the sensors 
	}
}
