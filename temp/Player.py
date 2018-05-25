class Player:
	m_state                = None
    m_id_player            = None
    m_radial_len           = None
    m_current_angle        = None
    m_max_angle_left_side  = None
    m_max_angle_right_side = None
    m_mac_adress           = None

    def __init__( self, MAC_ADRESS ):
    	self.m_mac_adress  = MAC_ADRES

    def set_state_player( self, state ):
        self.m_state = state

    def set_id_player( self, id ):
        self.m_id_player = id
    
    def sets_radial_len( self, start_pos , len ):
    	self.m_current_angle = start_pos
    	self.m_radial_len = len

    def set_angle_limit( self, left, right ):
        self.m_max_angle_left_side  = left
        self.m_max_angle_right_side = right   

    def move( self, new_pos ):
    	m_current_angle = new_pos
    	
    def get_current_pos( self ):
        return self.m_current_angle	    

    def get_mac_adress( self ):
        return self.get_mac_adress

    
    


