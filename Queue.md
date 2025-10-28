Request: 
	position
	direction

Direction:
	Up = 1
	Stationary = 0
	Down = -1
### Elevator Stationary
Old State
	old target: N/A
	old position: 0
	old direction: 0

(INPUT) New Request: 
	position: 4
	direction: 1 

(OUTPUT) New State
	new target: 4
	new position: moving
	new direction: 1
	
### Elevator Moving Up
Old State
	old target: 4
	old position: 2.1
	old direction: 1

(INPUT) New Request
	position: 3
	direction: 1

(OUTPUT) New State
	new target: 3
	new position: moving
	new direction: 1

### Elevator Moving Down
Old State 
	old target: 0
	old position:  4
	old direction: -1

(INPUT) New Request
	position: 2
	direction: 1

(OUTPUT) New State
new target: 0
new position: 4
new direction: -1