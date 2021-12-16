task_compile() {
	make
}

task_clean_compiled() {
	make clean
}

task_insert_module() {
	sudo insmod hw2_module.ko
}

task_list_modules() {
	lsmod | grep hw2
}

task_remove_module() {
	sudo rmmod hw2_module.ko
}

task_kernel_log() {
	dmesg
}

task_edit() {
    vi ./hw2_module.c
}


task_cleanup() {
	task_remove_module
	task_clean_compiled
}

task_run() {
	task_cleanup
	task_compile
	task_insert_module
	task_kernel_log
}
