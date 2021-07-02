V2V_Communication_crossway()
{
	communication_active();//Vehicles communication is stablished 
	calcullate_speed();// Vehicles calculate their speed and prepare it for sharing
	get_position();// Vehicles get their position infomation and prepare it for sharing 
	get_trajectory();// Vehicles get their trajectory infomation and prepare it for sharing
	check_requests();// Vehicles can check if they have received any request for information sharing 

	if(info_request=true)// true: A request for sharing information is received
	{
		if (request=speed)
		send_speed();//Vehicles send their speed information
		elseif(reuest=position)
		send_position();// Vehicles send their position infomation
		else
		send_trajectory();//Vehicles send their trajectory infomation
	}
	else
	wait_r();//vehicles are waitting for a  request to share information
	if(info_get=true)// true: A request for getting information is received
 {
		get_info()//vehicles are   requestting a specific information
	{
		int info_type;
		switch (info_type)
		{
			case 1:
			get_speed();//Vehicles ask for speed information
			break;
			case 2:
			get_position();// Vehicles ask for position infomation
			break;
			case 3:
		    get_trajectory();//Vehicles ask for trajectory infomation	
				
		}
	}
    }
    else
    wait_g();//vehicles are waitting to get information
    
}
