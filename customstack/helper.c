#include "helper.h"

void *ParseStackConfig(void)
{
	struct rte_cfgfile *file = rte_cfgfile_load(STACK-CFG, 0);

	/* get section name EAL */
	if (rte_cfgfile_has_section(file, "EAL")) {
		printf(" section (EAL); count %d", rte_cfgfile_num_sections(file, "EAL", sizeof("EAL") - 1));
		printf(" section (EAL) has entries %d", rte_cfgfile_section_num_entries(file, "EAL"));

		int n_entries = rte_cfgfile_section_num_entries(file, "EAL");
		struct rte_cfgfile_entry entries[n_entries];

		if (rte_cfgfile_section_entries(file, "EAL", entries, n_entries) != -1) {
			argument_count += n_entries * 2;
			printf(" argument_count %d", argument_count);

			for (int i = 0; i < n_entries; i++) {
				printf(" - entries[i].name: (%s) entries[i].value: (%s)", entries[i].name, entries[i].value);
				snprintf(argument[i * 2 + 1], 32, "%s", entries[i].name);
				snprintf(argument[i * 2 + 2], 32, "%s", entries[i].value);
				printf(" - argument: (%s) (%s)", argument[i * 2 + 1], argument[i * 2 + 2]);
			}
		}
	}

	/* get section name PORT-X */
	for (int i = 0; i < RTE_MAX_ETHPORTS; i++) {
		char port_section_name[15] = {""};

		sprintf(port_section_name, "%s%d", "PORT-", i);
		if (rte_cfgfile_has_section(file, port_section_name)) {
			int n_port_entries = rte_cfgfile_section_num_entries(file, port_section_name);

			printf(" %s", port_section_name);
			printf(" section (PORT) has %d entries", n_port_entries);

			struct rte_cfgfile_entry entries[n_port_entries];
			if (rte_cfgfile_section_entries(file, port_section_name, entries, n_port_entries) != -1) {

				for (int j = 0; j < n_port_entries; j++) {
					printf(" %s name: (%s) value: (%s)", port_section_name, entries[j].name, entries[j].value);

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
				}
			}
		}
	}

	/* get section name MEMPOOL-PORT */
	if (rte_cfgfile_has_section(file, "MEMPOOL-PORT")) {
		printf(" section (MEMPOOL-PORT); count %d", rte_cfgfile_num_sections(file, "MEMPOOL-PORT", sizeof("MEMPOOL-PORT") - 1));
		printf(" section (MEMPOOL-PORT) has entries %d", rte_cfgfile_section_num_entries(file, "MEMPOOL-PORT"));

		int n_entries = rte_cfgfile_section_num_entries(file, "MEMPOOL-PORT");
		struct rte_cfgfile_entry entries[n_entries];

		if (rte_cfgfile_section_entries(file, "MEMPOOL-PORT", entries, n_entries) != -1) {
			for (int j = 0; j < n_entries; j++) {
				printf(" - entries[i] name: (%s) value: (%s)", entries[j].name, entries[j].value);

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
			}
		}
	}

	rte_cfgfile_close(file);

}
