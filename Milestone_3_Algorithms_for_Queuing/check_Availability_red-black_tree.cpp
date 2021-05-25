check_Availability ()
{
	cell_available=true;//check if a cell(square) is available for modification
	set_node_x();
	// rtd - requested time of departure 
	// X is a node 
	//x_at - is the arrival time of node x
	//x_dt-is the departure time of node x
	//x_l - is the left node of x 
	//x_r is the right node of x
	While(x!=null&&cell_avaiable=true)//Check if cell is available for operation and also if a new node is ready to be created
	{
	
	if(rtd<=x_at)//check if arrival time of the node x is smaller than request time of departure 

	x=x_l ;//put the new x node to the left of the red-black tree as a leaf
	
    else if(rtd=>x_dt)//check if departure time of the node x is bigger than request time of departure 
    
    x=x_r//put the new x node to the left of the red-black tree as a leaf
    
    else
    
    cell_available=false;//we put the cell as used 
}
return x;
udate_tree();//put the new x node in the red-black tree and we update the tree
}
