编译：

./configure --host=arm-linux --prefix=INSTALLPATH --enable-static --disable-shared --with-ksource=KERNELSOURCEPATH

Networking support
Networking options  --->  
	[*]   IP: advanced router 
	
	[*] Network packet filtering framework (Netfilter)  --->  
	Core Netfilter Configuration  ---> 
		<*> Netfilter connection tracking support
		<*>   NetBIOS name service protocol support
		-*- Netfilter Xtables support (required for ip_tables)  
	
	 IP: Netfilter Configuration  --->   
	 			 <*> IPv4 connection tracking support (required for NAT)  
	 			 <*> IP tables support (required for filtering/masq/NAT)
	 			 <*>   IPv4 NAT 
	 			 <*>     MASQUERADE target support
	 			 <*>     NETMAP target support
	 			 <*>     REDIRECT target support  

NAT command:
iptables -t nat -A POSTROUTING -s 192.168.0.0/255.255.255.0 -o eth1 -j MASQUERADE
