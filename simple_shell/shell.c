#include "seashell.h"

char *get_filepath(char *command)
{
	/*char *tmp = NULL;*/
	dir_list_t *list = NULL;
	dir_list_t *current = NULL;

	list = make_path_list();
	current = list;

	while (current)
	{
		/*tmp = malloc(256);
		  if (!tmp)
		  {
		  free_list(&list);
		  return (NULL);
		  }*/
		_strcpy(tmp, current->dir_path);
		_strcat(tmp, "/");
		_strcat(tmp, command);

		if (!access(tmp, F_OK))
		{
			free_list(&list);
			return (tmp);
		}
		current = current->next;
	}
	free_list(&list);
	return (NULL);
}


char **make_argv(char *string)
{
	/*char **array = NULL;*/
	char *find_delim = NULL, *token = NULL;
	int count = 0;
	int i;

	find_delim = string;

	while (*find_delim)
	{
		if (*find_delim == ' ')
			count++;
		find_delim++;
	}
	count++;

	array = malloc(sizeof(char *) * (count + 1));
	array[count] = NULL;

	i = 0;

	token = strtok(string, " ");
	if (!token)
	{
		free(array);
		return (NULL);
	}
	if (*token != '/')
	{
		array[i] = get_filepath(token);
		if (!array[i])
		{
			free(array);
			return (NULL);
		}
	}
	else
		array[i] = token;
	i++;

	while (token)
	{
		token = strtok(NULL, " ");
		array[i] = token;
		i++;
	}

	return(array);
}

void exec_command(char *usr_input)
{
	char **argv = NULL;
	pid_t is_parent, j;

	argv = make_argv(usr_input);
	if (!argv)
	{
		printf("Something is going terribly wrong.");
		free(usr_input);
		return;
	}

        is_parent = fork();
        if (is_parent == 0)
	{
		j = execve(argv[0], argv, NULL);
		if (j == -1)
		{
			printf("./shell: No such file or directory\n");
			free(argv);
			free(usr_input);
			return;
		}
	}
        wait(NULL);
	free(argv);
	free(usr_input);
	return;
}

int main(void)
{
	char *newline = NULL;
	size_t bufsize = 1024;
	ssize_t p;

	while (1)
	{
		_printf("$ ");
		usr_input = malloc(1024);
		if(!usr_input)
			return (-1);
		p = getline(&usr_input, &bufsize, stdin);
		if (p == -1)
		{
			free(usr_input);
			exit(0);
		}
		newline = _strchr(usr_input, '\n');
		if (newline)
			*newline = '\0';
		if(!*usr_input)
		{
			free(usr_input);
			continue;
		}
		if (!strcmp(usr_input, "exit"))
			break;
		if (!strcmp(usr_input, "env"))
		{
			env_builtin();
			free(usr_input);
			continue;
		}
		exec_command(usr_input);
	}
	free(usr_input);
	return (0);
}
