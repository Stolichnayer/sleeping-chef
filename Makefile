all: 
	gcc -pthread restaurant.c -o restaurant 

run: 
	./restaurant 12

clean:
	rm -f restaurant