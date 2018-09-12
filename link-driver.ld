SECTIONS {
	.text : {
		*(.text.startup); /* place main() at 0x00 */
		*(.text);
	}
}