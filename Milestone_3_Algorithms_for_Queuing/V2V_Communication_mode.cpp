V2V_Communication_mode()
{
	
check_range();// The distance between vehicles is check

 if(vehicle_in_range=true)	// true: vehicles are in distance that allow them to communicate direct without the cloud base system.
 
 {
 printf("Vehicles are in range and can  communicate directly")
 On_board_Communication();// On board communication is running
 
 }
 
 else
 {
 	printf("Vehicles are  not in range and can  communicate through cloud based system  ")
 	cloud_communication();// Cloud communication is running
 }
 return mode;
}
