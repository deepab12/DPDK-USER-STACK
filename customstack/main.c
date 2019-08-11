#include "helper.h"

extern uint16_t argument_count;
extern char argument[DPDK_MAXARGS][32];
char *args[DPDK_MAXARGS];
static int
lcore_hello(__attribute__((unused)) void *arg)
{
	unsigned lcore_id;
	lcore_id = rte_lcore_id();
	printf("hello from core %u\n", lcore_id);
	return 0;
}

/* Parse the argument given in the command line of the application */
static int
stack_parse_args(int argc, char **argv)
{
	char *prgname = argv[0];
	char **argvopt;
	int option_index, opt;
	static const char short_options[] =
        "f:"  /* cfg file */
        "T:"  /* timer period */
        ;

	argvopt = argv;

	RTE_LOG(INFO, STACK, " prgname (%s)\n", prgname);

	while ((opt = getopt_long(argc, argvopt, short_options, NULL, &option_index)) != EOF) {
		switch (opt) {
		case 'f':
			RTE_LOG(INFO, STACK, " file (%s) index (%d)\n", optarg, option_index);
			if (ParseStackConfig(optarg) == NULL)
				return -1;
			break;

		case 0:
			break;

		default:
			return -1;
		}
	}

	return 0;
}

int
main(int argc, char **argv)
{
	int ret;
	unsigned lcore_id;

	if (stack_parse_args(argc, argv) != 0) {
		RTE_LOG(ERR, STACK, " fail to read config\n");
		return -1;
	}

	RTE_LOG(INFO, STACK, " EAL arg count (%u) \n", argument_count);
	for (int i = 0; i < argument_count; i++)
		RTE_LOG(INFO, STACK, " args (%s) \n", argument[i]);

	for (int j = 0; j < argument_count; j++)
		args[j] = argument[j];

	ret = rte_eal_init(argument_count, args);
	if (ret < 0)
		rte_panic("Cannot init EAL\n");

	/* call lcore_hello() on every slave lcore */
	RTE_LCORE_FOREACH_SLAVE(lcore_id) {
		rte_eal_remote_launch(lcore_hello, NULL, lcore_id);
	}

	/* call it on master lcore too */
	lcore_hello(NULL);

	rte_eal_mp_wait_lcore();
	return 0;
}
