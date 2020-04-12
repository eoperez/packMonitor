# Pack Monitor
This project is a lean version of [diyBMS](https://github.com/stuartpittaway/diyBMSv4) by [stuartpittaway](https://github.com/stuartpittaway) and [cell-monitor](https://github.com/scttnlsn/cell-monitor) by [scttnlsn](https://github.com/scttnlsn). If you are looking to do top balancing and have soldering skills I would encourage you to look at those projects.

If you are looking for:
* solar system application.
* simple voltage sensor
* simple temperature sensor
* MQTT Publishing
* Node-Red Integration

then, this project might be of your interest.

![Real Time Dashboard](images/realTimeDashboard.png?raw=true "Real Time Dashboard")

#### __Why packMonitor instead of cell-monitor?__ 
Simply... to avoid confusion. By definition a cell is the smallest structural and functional unit of an system (in this context).  In my case, I'm creating a powerwall made up of 18650 batteries, those are grouped in parallel and then put in series to obtain final voltage. So for this context then:
* Cell = a single 18650 battery
* Single Battery (a Pack) = a group of batteries in parallel
* Battery Bank = a group of packs in series to achieve a given voltage.

In this scenario, I want to measure the pack.

## What do you need?
To keep project simple I'm using development boards that require little to non soldering:
* __Balancer__: up to you. I'm using Daligreen 1S.
* __Digispark ATTINY85__: this should be under $1.50 USD each. You'll need one of these for each pack.
* __PCB Board__: to reduce number of cables - normally 15 for $5.30 + shipping.
* __Temp Sensor__: Optional - around $1.50 USD.
* __NodeMCU ESP32-S__: this should be under $10.00 USD. You just need one.
* __Ribbon Cable 28 AWG__: really depends on the distance. I found a kit of 6 color with 32.8 feet each (color) under $15.00 USD.
* __Pin Connectors__: depends on how many monitors you are going to use. I purchased a kit under $6.00 (a lot more than what I needed.)
* __Computer__: is obvious, but is needed :) - in my case I use it as MQTT broker and for automation/data display.

## Connections
![Connections](images/connections.png?raw=true "Connections")
## Becoming a real BMS? 
This is a sensitive topic in the community. Based on several articles a BMS (Battery Management Systems) should: 
* __Monitor__: verify voltage, temperature, current, state of charge (SOC) and state of health (SOH)  includes: total and individual cells/packs values, alert on minimum and maximum values. Example: you want to monitor: each individual pack voltage, temperature, current and health and the average, collective values for the overall bank.
* __Protect__: connect/disconnect to prevent overcharge and undercharge using voltage & current protection based on settings. Cooling flow in case of high temperatures.
* __Optimize__: minimize energy wasting. Essentially moving energy effectively in, out and between packs. This is where balancing sits.

This project just will cover part of the monitoring but with some extra items like:

 * Node-Red
 * Node-Red-Dashboards 
 * MQTT Broker

You can get something like this:

My final goal is to have a true BMS combining functionality of the equipment I have in my solar setup,like this: 
![Full BMS](images/FullBMS.png?raw=true "Full BMS")
> I'll link the other projects related to final goal here.