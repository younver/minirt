# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cfeijoo <cfeijoo@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2014/03/02 14:37:36 by cfeijoo           #+#    #+#              #
#    Updated: 2023/12/20 14:25:42 by younver          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #
NAME = rt

INCLUDEFOLDERS = -I./includes/ -I./libft/includes/ -I/usr/X11/include -I./libmlx/
LIBFOLDERS = -L./libft/ -L./libmlx/ -L/usr/X11/lib
LIBS = -lmlx -lXext -lX11 -lft -lm -lpthread

CC = clang
CFLAGS = -Wall -Werror -Wextra -O3 -std=c99

SOURCES_FOLDER = sources/
OBJECTS_FOLDER = objects/

SOURCES =	rt.c										\
			camera/camera_transformations.c				\
			camera/camera_init.c						\
			object/creation.c							\
			\
			scene_parser/parser.c						\
			scene_parser/init_parser.c					\
			scene_parser/command_camera.c				\
			scene_parser/command_add_sphere.c			\
			scene_parser/command_add_cone.c				\
			scene_parser/command_add_cylinder.c			\
			scene_parser/command_add_plane.c			\
			scene_parser/command_add_light.c			\
			scene_parser/command_define_xyz.c			\
			scene_parser/command_define_rotxyz.c		\
			scene_parser/command_define_normxyz.c		\
			scene_parser/command_define_rgb.c			\
			scene_parser/command_define_radius.c		\
			scene_parser/command_define_ambient.c		\
			scene_parser/command_define_opacity.c		\
			scene_parser/command_define_diffuse.c		\
			scene_parser/command_define_specular.c		\
			scene_parser/command_define_refraction.c	\
			scene_parser/command_define_reflection.c	\
			scene_parser/command_define_intensity.c		\
			scene_parser/command_define_color.c			\
			\
			scene_parser/command_define_axisx.c			\
			scene_parser/command_define_axisy.c			\
			scene_parser/command_define_axisz.c			\
			scene_parser/command_define_antialias.c		\
			scene_parser/command_define_recursivity.c	\
			scene_parser/command_define_diaphragm.c		\
			scene_parser/command_define_width.c			\
			scene_parser/command_define_height.c		\
			\
			interpreter/interpreter.c					\
			interpreter/command_list.c					\
			interpreter/command_remove.c				\
			interpreter/command_add.c					\
			interpreter/command_export.c				\
			interpreter/command_antialias.c				\
			interpreter/command_render.c				\
			interpreter/command_editmode.c				\
			interpreter/command_recursivity.c			\
			interpreter/command_unblock.c				\
			interpreter/init_interpreter.c				\
			export/export_image.c						\
			export/export_scene.c						\
			mouse/mouse.c								\
			mouse/motion_notify.c						\
			ray_throw.c									\
			equations/get_normal.c						\
			equations/equation_cone.c					\
			equations/equation_cylinder.c				\
			equations/equation_sphere.c					\
			equations/equation_plane.c					\
			equations/positive_smallest.c				\
			equations/matrix.c							\
			equations/change_ray.c						\
			phong_shading.c								\
			render_thread.c								\
			render_to_image.c							\
			light_diaphragm.c							\
			keyboard/keyboard.c							\
			keyboard/keypress_hook.c					\
			keyboard/keyrelease_hook.c					\
			keyboard/check_pressed_keys.c				\
			pixel.c										\

OBJECTS =	$(SOURCES:.c=.o)

OBJECTS = $(SOURCES:.c=.o)
OBJECTS := $(subst /,__,$(OBJECTS))
OBJECTS := $(addprefix $(OBJECTS_FOLDER), $(OBJECTS))
SOURCES := $(addprefix $(SOURCES_FOLDER),$(SOURCES))

# Colors
NO_COLOR =		\x1b[0m
OK_COLOR =		\x1b[32;01m
ERROR_COLOR =	\x1b[31;01m
WARN_COLOR =	\x1b[33;01m
SILENT_COLOR =	\x1b[30;01m

.PHONY: all re init clean fclean

all: complibs $(NAME)

linux :
	@$(eval INCLUDEFOLDERS := -I./includes/ -I./libft/includes/ -I/usr/X11/include/ -I/usr/include/X11)
	@$(eval LIBFOLDERS := -L./libft/ -L/usr/X11/lib/ -L/usr/X11/include)

$(OBJECTS_FOLDER)%.o:
	@$(CC) -c $(subst .o,.c,$(subst $(OBJECTS_FOLDER),$(SOURCES_FOLDER),$(subst __,/,$@))) $(INCLUDEFOLDERS) $(CFLAGS) $(MACROS) -o $@
	@printf "$(OK_COLOR)✓ $(NO_COLOR)"
	@echo "$(subst .o,.c,$(subst $(OBJECTS_FOLDER),$(SOURCES_FOLDER),$(subst __,/,$@)))"

$(NAME) : $(OBJECTS)
	@printf "$(SILENT_COLOR)Compiling $(NAME)...$(NO_COLOR)"
	@$(CC) $(OBJECTS) $(INCLUDEFOLDERS) $(LIBFOLDERS) $(LIBS) $(CFLAGS) -o $(NAME)
	@echo " $(OK_COLOR)Successful ✓$(NO_COLOR)"

complibs :
	@make -C libmlx/
	@make -C libft/ usemath all

clean :
	@rm -f $(OBJECTS)
	@echo "$(SILENT_COLOR)$(NAME) : Cleaned Objects$(NO_COLOR)"

fclean : clean
	@rm -f $(NAME)
	@echo "$(SILENT_COLOR)$(NAME) : Cleaned Program$(NO_COLOR)"
	@make -C "libft" fclean

re : fclean all
