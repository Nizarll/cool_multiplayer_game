main: main.c
	g++ main.c -lraylib -lm src/renderer.c src/entity.c src/dynarr.c src/ease.c -o renderer
