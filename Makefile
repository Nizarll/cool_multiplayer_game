main: main.c
	gcc main.c -lraylib -lm src/renderer.c src/ease.c -o renderer
