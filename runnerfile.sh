task_comp() {
	make
}

task_clean() {
	make clean
}

task_insert() {
	sudo insmod hw2_module.ko
}

task_list() {
	lsmod | grep hw2
}

task_remove() {
	sudo rmmod hw2_module.ko
}

task_log() {
	dmesg
}

task_all() {
	comp
	insert
	log
}
