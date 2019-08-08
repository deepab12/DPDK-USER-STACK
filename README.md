# DPDK-USER-STACK
experimentation is creating a user stack using DPDK PMD and libraries.

## Motivation
- [ ] understand closely how Linux stack works.
- [ ] use netlink for synch between userspace stack and Linux stack
- [ ] Bifurcate with TAP or XDP for desired traffic to custom stack

# how it is donw

CMD: ```./myapp <mac address> <ip address> <tcp port number>```

## Tap interfaces:
- dpdk tap interface: dpdk-tap-eth1
- application tap interface: knl-tap-eth1

## How it works
```
	Set ipaddress for 'dpdk-tap-eth1' and 'knl-tap-eth1'
	Use dpdk interface to get packet from dpdk nic
	check mac, ip and protocl type (TCP)
	check if reassembly should happen - complete packet
	tcp port check, if intentend packet send this packet to kernel (dpdk-tap-eth1)
	Use linux routing to foward traffic between tap interfaces
```
