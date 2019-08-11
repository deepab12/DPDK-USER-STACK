#include "helper.h"

uint16_t argument_count = 1;
char argument[DPDK_MAXARGS][32] = {{"./mystack"}, {""}};

void *ParseStackConfig(char *CfgFile)
{
	struct rte_cfgfile *file = rte_cfgfile_load(CfgFile, 0);
	if (file == NULL) {
		RTE_LOG(ERR, STACK, " failed to get file (%s)", CfgFile);
		return file;
	}

	/* get section name EAL */
	if (rte_cfgfile_has_section(file, "EAL")) {
		RTE_LOG(DEBUG, STACK, " section (EAL); count %d\n", rte_cfgfile_num_sections(file, "EAL", sizeof("EAL") - 1));
		RTE_LOG(DEBUG, STACK, " section (EAL) has entries %d\n", rte_cfgfile_section_num_entries(file, "EAL"));

		int n_entries = rte_cfgfile_section_num_entries(file, "EAL");
		struct rte_cfgfile_entry entries[n_entries];

		if (rte_cfgfile_section_entries(file, "EAL", entries, n_entries) != -1) {
			for (int i = 0; i < n_entries; i++) {

				if (strlen(entries[i].name)) {
					RTE_LOG(DEBUG, STACK, " - entries[i].name: (%s)\n", entries[i].name);
					memcpy(argument[i * 2 + 1], entries[i].name, strlen(entries[i].name));
					argument_count += 1;
				}

				if (strlen(entries[i].value)) {
					RTE_LOG(DEBUG, STACK, " - entries[i].value: (%s)\n", entries[i].value);
					memcpy(argument[i * 2 + 2], entries[i].value, strlen(entries[i].value));
					argument_count += 1;
				}
			}
		}

		for (int i = 0; i < argument_count; i++)
			RTE_LOG(INFO, STACK, " - argument: (%s)\n", argument[i]);
	}

	/* get section name PORT-X */
	for (int i = 0; i < RTE_MAX_ETHPORTS; i++) {
		char port_section_name[15] = {""};

		sprintf(port_section_name, "%s%d", "PORT-", i);
		if (rte_cfgfile_has_section(file, port_section_name)) {
			int n_port_entries = rte_cfgfile_section_num_entries(file, port_section_name);

			RTE_LOG(INFO, STACK, " %s", port_section_name);
			RTE_LOG(INFO, STACK, " section (PORT) has %d entries\n", n_port_entries);

			struct rte_cfgfile_entry entries[n_port_entries];
			if (rte_cfgfile_section_entries(file, port_section_name, entries, n_port_entries) != -1) {

				for (int j = 0; j < n_port_entries; j++) {
					RTE_LOG(INFO, STACK, " %s name: (%s) value: (%s)\n", port_section_name, entries[j].name, entries[j].value);

#if 0
					if (strcasecmp("rx-queues", entries[j].name) == 0)
						dpdk_ports[i].rxq_count = atoi(entries[j].value);
					else if (strcasecmp("tx-queues", entries[j].name) == 0)
						dpdk_ports[i].txq_count = atoi(entries[j].value);
					else if (strcasecmp("mtu", entries[j].name) == 0)
						dpdk_ports[i].mtu = atoi(entries[j].value);
					else if (strcasecmp("rss-tuple", entries[j].name) == 0)
						dpdk_ports[i].rss_tuple = atoi(entries[j].value);
					else if (strcasecmp("jumbo", entries[j].name) == 0)
						dpdk_ports[i].jumbo = (strcasecmp(entries[j].value, "yes") == 0) ? 1 : 0;
					else if (strcasecmp("core", entries[j].name) == 0)
						dpdk_ports[i].lcore_index = atoi(entries[j].value);
#endif
				}
			}
		}
	}

	/* get section name MEMPOOL-PORT */
	if (rte_cfgfile_has_section(file, "MEMPOOL-PORT")) {
		RTE_LOG(INFO, STACK, " section (MEMPOOL-PORT); count %d\n", rte_cfgfile_num_sections(file, "MEMPOOL-PORT", sizeof("MEMPOOL-PORT") - 1));
		RTE_LOG(INFO, STACK, " section (MEMPOOL-PORT) has entries %d\n", rte_cfgfile_section_num_entries(file, "MEMPOOL-PORT"));

		int n_entries = rte_cfgfile_section_num_entries(file, "MEMPOOL-PORT");
		struct rte_cfgfile_entry entries[n_entries];

		if (rte_cfgfile_section_entries(file, "MEMPOOL-PORT", entries, n_entries) != -1) {
			for (int j = 0; j < n_entries; j++) {
				RTE_LOG(INFO, STACK, " - entries[i] name: (%s) value: (%s)\n", entries[j].name, entries[j].value);

#if 0
				if (strcasecmp("name", entries[j].name) == 0)
					rte_memcpy(dpdk_mempool_config.name, entries[j].value, sizeof(entries[j].value));
				if (strcasecmp("n", entries[j].name) == 0)
					dpdk_mempool_config.n = atoi(entries[j].value);
				if (strcasecmp("elt_size", entries[j].name) == 0)
					dpdk_mempool_config.elt_size = atoi(entries[j].value);
				if (strcasecmp("private_data_size", entries[j].name) == 0)
					dpdk_mempool_config.private_data_size = atoi(entries[j].value);
				if (strcasecmp("socket_id", entries[j].name) == 0)
					dpdk_mempool_config.private_data_size = atoi(entries[j].value);
#endif
			}
		}
	}

	rte_cfgfile_close(file);
	return file;
}
