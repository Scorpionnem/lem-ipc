![lem-ipc-gif](https://github.com/user-attachments/assets/8d36d9ae-9f29-4633-b5d3-c7cf39ec9a49)

lem-ipc is a project in the 42 advanced. It is about learning inter process communication.

## Compilation / usage

```sh
make
./lemipc-visualizer
```

The Makefile will compile 2 binaries, one for the program itself and one for the visualizer.

The visualizer is an all in one tool to test out my lem-ipc
These are the keybinds you can use in the visualizer:
- S : Run a lemipc process in the background
- Scroll Up/Down : Change the team of the processes launched using S
- Left Click: Place/Destroy a wall

# Functions used

In this project we have to use System V IPC's so we use these functions:

### ftok
Creates a key that can be used in all the other functions we use
### msgget
Creates a message queue used to communicate between processes
I use this function to make my processes exchange informations about targets or who should be the team leader.
### semget
Creates a semaphore
I use this function to avoid data races between processes, it will be used to lock the shared memory segment.
### shmget
Creates a shared memory segment

In the 3 above functions, the first process will have to initiate the IPC's and any other process should only get them.

### shmat
Returns a pointer to the shared memory segment created using shmget

# Rules of the project

### Technical constraints

In this project, each cell should be a process, that's why we use IPC's as each process is able to access the same memory, for example, in my project they are able to access the map and the state of the game in this struct wich is stored in the shared memory.

```C
typedef struct s_shared
{
	int		board[BOARD_SIZE][BOARD_SIZE];
	int		counter;
	bool	paused;
}	t_shared;
```

The board will store the game's state, a 0 is empty space, -1 is a wall and anything > 0 is a player with its team being the value each tile can only contain 1 element.

### Movement/Death checks

Cells should move up/down/left/right, no diagonals.

```

C : Cell
X : Place it can move to

 X 
XCX
 X 
```

Before moving, a cell should check if it is dead. (It has to check itself)

To be dead it should have 2 enemies of the same enemy team around it (all 8 tiles around) like this:

```

C : Cell
X : Enemy cell

XXX
XCX
XXX
```

If a team leader dies, it send 1 message to its team's message queue to tell one of its teammate to become the team leader.

### Targetting

Players should not move randomly, they have to have to be at least a little smart, for this I chose to make them move to the tile next to them thats the closest to the target.

To chose who they target I chose to have a "team leader", when a process joins a team it checks if its the first of its team, if so it becomes a team leader.
Its job will be to tell its team who to target, to chose the target it will take the closest enemy cell to itself.
To communicate it uses the message queue to send a message to its team with this format

```
TARGET X Y
```
To communicate only to its teammates, it will send the message with its team as the type. And its teammates will listen to messages with their team as the type.

```C
t_message	msgdata;

...

msgdata.mtype = team;
msgsnd(msgqid, &msgdata, sizeof(msgdata.mtext), IPC_NOWAIT);
```

### Game rules

For the "game" to end, there should be 1 team left on the board, if there are 2 teams with 1 cell alive, its a special edge case where I chose to just end the game.

# Visualizer

For the bonus of this project we are asked to make a graphical interface, I recommend to do this even if you only planned to do mandatory.

The concept is easy, we have a window that displays the board.
All it needs to do is access the shared memory, read and print to the window.

I chose to have a separate executable but you could also have the first process open the window and visualize at the same time as playing. You would have to think about what happens when it dies tho.

I used the 42's minilibx but any library can be used.

# Bonuses

For the bonuses I went for some small but useful ones such as:

- Displaying infos about the game (Player count, teams count)
- Being able to pause the game
- Being able to spawn processes from the visualizer (So you dont have to start 100 terminals)
- Displaying crowns on the biggest team's members
- Placing/Breaking walls
