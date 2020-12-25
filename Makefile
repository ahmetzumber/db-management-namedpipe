make: program database

program: program.c
	 gcc program.c -o program

database: database.c 
	gcc database.c -o database 

kaydet: kaydet.c 
	gcc kaydet.c -o kaydet


