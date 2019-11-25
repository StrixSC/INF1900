Port setups:

DDRA: 
    6 4 2 0
    -------
    1 0 0 0     6 = Trigger du sonar
    0 0 0 0     0,1,2,3,4,5,7 = Cytron Line Tracker
    -------
    7 5 3 1

DDRB:
    6 4 2 0
    -------
    1 1 1 1     0,1 = Lumiere DEL libre
    1 1 1 1     2,3,4,5,6,7 = Unused
    -------
    7 5 3 1 

DDRC:
    6 4 2 0
    -------
    x x x x     DDRC Dedie au LCD Display. Les ports ne sont pas configures par nous.    
    x x x x     
    -------
    7 5 3 1 

DDRD: 
    6 4 2 0
    -------
    1 1 0 0     4,5,6,7 = Roues moteurs
    1 1 0 0     0,1,2,3 = boutons poussoirs.
    -------
    7 5 3 1