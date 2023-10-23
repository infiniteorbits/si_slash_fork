#include <slash/slash.h>
#include <slash/optparse.h>
#include <string.h>

int slash_run(struct slash *slash, char * filename, int printcmd) {

    /* Read from file */
	FILE * stream = fopen(filename, "r");
	if (stream == NULL) {
        printf("  File %s not found\n", filename);
		return SLASH_EIO;
    }

    char line[512];
    while(fgets(line, sizeof(line), stream)) {
        
        /* Strip newline */
        line[strcspn(line, "\n")] = 0;

        /* Skip short lines */
        if (strlen(line) <= 1)
            continue;

        /* Skip comments */
        if (line[0] == '/') {
            continue;
        }

        if (printcmd)
            printf("  run: %s\n", line);

        slash_execute(slash, line);
        slash_history_add(slash, line);
    }

    fclose(stream);

    return SLASH_SUCCESS;

}

static int cmd_run(struct slash *slash) {

    optparse_t * parser = optparse_new("run", "<filename>");
    optparse_add_help(parser);

    int argi = optparse_parse(parser, slash->argc - 1, (const char **) slash->argv + 1);
    if (argi < 0) {
        optparse_del(parser);
	    return SLASH_EINVAL;
    }

	/* Check if name is present */
	if (++argi >= slash->argc) {
		printf("missing parameter filename\n");
        optparse_del(parser);
		return SLASH_EINVAL;
	}

	char * name = slash->argv[argi];

    printf("Running %s\n", name);

    int res = slash_run(slash, name, 1);
    optparse_del(parser);
    return res;

}

slash_command(run, cmd_run, "<file>", NULL);
	