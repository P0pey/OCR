# ifndef GTK_H_
# define GTK_H_

# include <err.h>
# include <stdlib.h>
# include <stdio.h>
# include <gtk/gtk.h>
# include <SDL/SDL.h>
# include <SDL/SDL_image.h>
# include "../NeuralNetwork/ocr.h"
# include "../NeuralNetwork/layer.h"
# include "../NeuralNetwork/train.h"
# include "../NeuralNetwork/neural_network.h"
# include "../segmentation/sdl_operations.h"
# include "../segmentation/segmentation.h"
# include "../segmentation/rlsa.h"

typedef struct
{
	GtkBuilder *builder;
	gpointer user_data;
} SGlobalData;

#define UNUSED(x) (void)(x)

void save_text(GtkButton *button, GtkTextBuffer *buffer);

void load_images(GtkButton *button, GtkImage *image);

int trainNN();

void openFile(GtkButton *button, GtkLabel *text_label);

int launchOCR(GtkButton *button, GtkTextBuffer *buffer);

void create_window(int argc, char *argv[]);

# endif
