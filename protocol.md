# get_voltage  
* Args: None  
* Returns: voltage(1024/11V):Float
* Example:  
<- `{"cmd":"get_voltage"}`  
-> `{"voltage": 233}`  

# fuck_relay
* Args: relay_num:Int
* Returns: fuck_status:Int
* Example:  
<- `{"cmd":"fuck_relay",args:{"relay_num": 1}}`  
-> `{"fuck_status": 0}`  (Success)  
-> `{"fuck_status": 1}`  (Invalid relay number)  
-> `{"fuck_status": 2}`  (Relay is already on)  

# get_relays_status  
* Args: None  
* Returns: relays_status: [status:Int]  
* Example:  
<- `{"cmd":"get_relays_status"}`  
-> `{"relays_status": [0,1,1,1]}`  (0-off, 1-on)

# set_time_length  
* Args: time_length(ms):Int  
* Returns: time_set_status:Int  
* Example:  
<- `{"cmd":"set_time_length",args:{"time_length": 1919}}`  
-> `{"time_set_status": 0}`  (Success)  

# get_time_length
* Args: None
* Returns: time_length(ms):Int
* Example:  
<- `{"cmd":"get_time_length"}`  
-> `{"time_length": 810}`  

