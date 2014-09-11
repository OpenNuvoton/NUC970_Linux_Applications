cmd_/home/ccwang/bsp/git/nuc970/apps/DPO_RT3070_LinuxSTA_V2.3.0.2_20100412/os/linux/../../sta/sta_cfg.o := arm-linux-gcc -Wp,-MD,/home/ccwang/bsp/git/nuc970/apps/DPO_RT3070_LinuxSTA_V2.3.0.2_20100412/os/linux/../../sta/.sta_cfg.o.d  -nostdinc -isystem /usr/local/arm_linux_4.3.2/lib/gcc/arm-none-linux-gnueabi/4.3.2/include -I/home/ccwang/bsp/git/nuc970/linux-3.10.y/arch/arm/include -Iarch/arm/include/generated  -Iinclude -I/home/ccwang/bsp/git/nuc970/linux-3.10.y/arch/arm/include/uapi -Iarch/arm/include/generated/uapi -I/home/ccwang/bsp/git/nuc970/linux-3.10.y/include/uapi -Iinclude/generated/uapi -include /home/ccwang/bsp/git/nuc970/linux-3.10.y/include/linux/kconfig.h -D__KERNEL__ -mlittle-endian -Iarch/arm/mach-nuc970/include -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Werror-implicit-function-declaration -Wno-format-security -fno-delete-null-pointer-checks -O2 -fno-omit-frame-pointer -mapcs -mno-sched-prolog -mabi=aapcs-linux -mno-thumb-interwork -marm -D__LINUX_ARM_ARCH__=5 -march=armv5te -mtune=arm9tdmi -msoft-float -Uarm -fno-stack-protector -fno-omit-frame-pointer -fno-optimize-sibling-calls -g -Wdeclaration-after-statement -Wno-pointer-sign -fno-strict-overflow -D__KERNEL__ -I/home/ccwang/bsp/git/nuc970/linux-3.10.y/include -I/home/ccwang/bsp/git/nuc970/apps/DPO_RT3070_LinuxSTA_V2.3.0.2_20100412/include -mlittle-endian -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -O2 -fno-omit-frame-pointer -fno-optimize-sibling-calls -fno-omit-frame-pointer -mapcs -mno-sched-prolog -mno-thumb-interwork -D__LINUX_ARM_ARCH__=5 -march=armv5te -mtune=arm926ej-s --param max-inline-insns-single=40000 -Uarm -Wdeclaration-after-statement -Wno-pointer-sign -DMODULE -DAGGREGATION_SUPPORT -DPIGGYBACK_SUPPORT -DWMM_SUPPORT -DLINUX -Wall -Wstrict-prototypes -Wno-trigraphs -DSYSTEM_LOG_SUPPORT -DLED_CONTROL_SUPPORT -DKTHREAD_SUPPORT -DCONFIG_STA_SUPPORT -DDBG -DWPA_SUPPLICANT_SUPPORT -DDOT11_N_SUPPORT -DDOT11N_DRAFT3 -DRTMP_MAC_USB -DRT30xx -DRT3070 -DRTMP_USB_SUPPORT -DRTMP_TIMER_TASK_SUPPORT -DRTMP_RF_RW_SUPPORT -DRTMP_EFUSE_SUPPORT  -DMODULE  -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(sta_cfg)"  -D"KBUILD_MODNAME=KBUILD_STR(rt3070sta)" -c -o /home/ccwang/bsp/git/nuc970/apps/DPO_RT3070_LinuxSTA_V2.3.0.2_20100412/os/linux/../../sta/sta_cfg.o /home/ccwang/bsp/git/nuc970/apps/DPO_RT3070_LinuxSTA_V2.3.0.2_20100412/os/linux/../../sta/sta_cfg.c

source_/home/ccwang/bsp/git/nuc970/apps/DPO_RT3070_LinuxSTA_V2.3.0.2_20100412/os/linux/../../sta/sta_cfg.o := /home/ccwang/bsp/git/nuc970/apps/DPO_RT3070_LinuxSTA_V2.3.0.2_20100412/os/linux/../../sta/sta_cfg.c

deps_/home/ccwang/bsp/git/nuc970/apps/DPO_RT3070_LinuxSTA_V2.3.0.2_20100412/os/linux/../../sta/sta_cfg.o := \
    $(wildcard include/config/apsta/mixed/support.h) \
  /home/ccwang/bsp/git/nuc970/apps/DPO_RT3070_LinuxSTA_V2.3.0.2_20100412/include/rt_config.h \
    $(wildcard include/config/h//.h) \
    $(wildcard include/config/sta/support.h) \
  /home/ccwang/bsp/git/nuc970/apps/DPO_RT3070_LinuxSTA_V2.3.0.2_20100412/include/rtmp_type.h \
  /home/ccwang/bsp/git/nuc970/apps/DPO_RT3070_LinuxSTA_V2.3.0.2_20100412/include/rtmp_os.h \
  /home/ccwang/bsp/git/nuc970/apps/DPO_RT3070_LinuxSTA_V2.3.0.2_20100412/include/os/rt_linux.h \
    $(wildcard include/config/5vt/enhance.h) \
  include/linux/module.h \
    $(wildcard include/config/sysfs.h) \
    $(wildcard include/config/modules.h) \
    $(wildcard include/config/unused/symbols.h) \
    $(wildcard include/config/module/sig.h) \
    $(wildcard include/config/generic/bug.h) \
    $(wildcard include/config/kallsyms.h) \
    $(wildcard include/config/smp.h) \
    $(wildcard include/config/tracepoints.h) \
    $(wildcard include/config/tracing.h) \
    $(wildcard include/config/event/tracing.h) \
    $(wildcard include/config/ftrace/mcount/record.h) \
    $(wildcard include/config/module/unload.h) \
    $(wildcard include/config/constructors.h) \
    $(wildcard include/config/debug/set/module/ronx.h) \
  include/linux/list.h \
    $(wildcard include/config/debug/list.h) \
  include/linux/types.h \
    $(wildcard include/config/uid16.h) \
    $(wildcard include/config/lbdaf.h) \
    $(wildcard include/config/arch/dma/addr/t/64bit.h) \
    $(wildcard include/config/phys/addr/t/64bit.h) \
    $(wildcard include/config/64bit.h) \
  include/uapi/linux/types.h \
  arch/arm/include/generated/asm/types.h \
  /home/ccwang/bsp/git/nuc970/linux-3.10.y/include/uapi/asm-generic/types.h \
  include/asm-generic/int-ll64.h \
  include/uapi/asm-generic/int-ll64.h \
  arch/arm/include/generated/asm/bitsperlong.h \
  include/asm-generic/bitsperlong.h \
  include/uapi/asm-generic/bitsperlong.h \
  /home/ccwang/bsp/git/nuc970/linux-3.10.y/include/uapi/linux/posix_types.h \
  include/linux/stddef.h \
  include/uapi/linux/stddef.h \
  include/linux/compiler.h \
    $(wildcard include/config/sparse/rcu/pointer.h) \
    $(wildcard include/config/trace/branch/profiling.h) \
    $(wildcard include/config/profile/all/branches.h) \
    $(wildcard include/config/enable/must/check.h) \
    $(wildcard include/config/enable/warn/deprecated.h) \
    $(wildcard include/config/kprobes.h) \
  include/linux/compiler-gcc.h \
    $(wildcard include/config/arch/supports/optimized/inlining.h) \
    $(wildcard include/config/optimize/inlining.h) \
  include/linux/compiler-gcc4.h \
    $(wildcard include/config/arch/use/builtin/bswap.h) \
  /home/ccwang/bsp/git/nuc970/linux-3.10.y/arch/arm/include/uapi/asm/posix_types.h \
  /home/ccwang/bsp/git/nuc970/linux-3.10.y/include/uapi/asm-generic/posix_types.h \
  include/linux/poison.h \
    $(wildcard include/config/illegal/pointer/value.h) \
  /home/ccwang/bsp/git/nuc970/linux-3.10.y/include/uapi/linux/const.h \
  include/linux/stat.h \
  /home/ccwang/bsp/git/nuc970/linux-3.10.y/arch/arm/include/uapi/asm/stat.h \
  include/uapi/linux/stat.h \
  include/linux/time.h \
    $(wildcard include/config/arch/uses/gettimeoffset.h) \
  include/linux/cache.h \
    $(wildcard include/config/arch/has/cache/line/size.h) \
  include/linux/kernel.h \
    $(wildcard include/config/preempt/voluntary.h) \
    $(wildcard include/config/debug/atomic/sleep.h) \
    $(wildcard include/config/prove/locking.h) \
    $(wildcard include/config/ring/buffer.h) \
  /usr/local/arm_linux_4.3.2/lib/gcc/arm-none-linux-gnueabi/4.3.2/include/stdarg.h \
  include/linux/linkage.h \
  include/linux/stringify.h \
  include/linux/export.h \
    $(wildcard include/config/have/underscore/symbol/prefix.h) \
    $(wildcard include/config/modversions.h) \
  /home/ccwang/bsp/git/nuc970/linux-3.10.y/arch/arm/include/asm/linkage.h \
  include/linux/bitops.h \
  /home/ccwang/bsp/git/nuc970/linux-3.10.y/arch/arm/include/asm/bitops.h \
  include/linux/irqflags.h \
    $(wildcard include/config/trace/irqflags.h) \
    $(wildcard include/config/irqsoff/tracer.h) \
    $(wildcard include/config/preempt/tracer.h) \
    $(wildcard include/config/trace/irqflags/support.h) \
  include/linux/typecheck.h \
  /home/ccwang/bsp/git/nuc970/linux-3.10.y/arch/arm/include/asm/irqflags.h \
  /home/ccwang/bsp/git/nuc970/linux-3.10.y/arch/arm/include/asm/ptrace.h \
    $(wildcard include/config/arm/thumb.h) \
  /home/ccwang/bsp/git/nuc970/linux-3.10.y/arch/arm/include/uapi/asm/ptrace.h \
    $(wildcard include/config/cpu/endian/be8.h) \
  /home/ccwang/bsp/git/nuc970/linux-3.10.y/arch/arm/include/asm/hwcap.h \
  /home/ccwang/bsp/git/nuc970/linux-3.10.y/arch/arm/include/uapi/asm/hwcap.h \
  include/asm-generic/bitops/non-atomic.h \
  include/asm-generic/bitops/fls64.h \
  include/asm-generic/bitops/sched.h \
  include/asm-generic/bitops/hweight.h \
  include/asm-generic/bitops/arch_hweight.h \
  include/asm-generic/bitops/const_hweight.h \
  include/asm-generic/bitops/lock.h \
  include/asm-generic/bitops/le.h \
  /home/ccwang/bsp/git/nuc970/linux-3.10.y/arch/arm/include/uapi/asm/byteorder.h \
  include/linux/byteorder/little_endian.h \
  include/uapi/linux/byteorder/little_endian.h \
  include/linux/swab.h \
  include/uapi/linux/swab.h \
  /home/ccwang/bsp/git/nuc970/linux-3.10.y/arch/arm/include/asm/swab.h \
  /home/ccwang/bsp/git/nuc970/linux-3.10.y/arch/arm/include/uapi/asm/swab.h \
  include/linux/byteorder/generic.h \
  include/asm-generic/bitops/ext2-atomic-setbit.h \
  include/linux/log2.h \
    $(wildcard include/config/arch/has/ilog2/u32.h) \
    $(wildcard include/config/arch/has/ilog2/u64.h) \
  include/linux/printk.h \
    $(wildcard include/config/early/printk.h) \
    $(wildcard include/config/printk.h) \
    $(wildcard include/config/dynamic/debug.h) \
  include/linux/init.h \
    $(wildcard include/config/broken/rodata.h) \
  include/linux/kern_levels.h \
  include/linux/dynamic_debug.h \
  include/linux/string.h \
    $(wildcard include/config/binary/printf.h) \
  include/uapi/linux/string.h \
  /home/ccwang/bsp/git/nuc970/linux-3.10.y/arch/arm/include/asm/string.h \
  include/linux/errno.h \
  include/uapi/linux/errno.h \
  arch/arm/include/generated/asm/errno.h \
  /home/ccwang/bsp/git/nuc970/linux-3.10.y/include/uapi/asm-generic/errno.h \
  /home/ccwang/bsp/git/nuc970/linux-3.10.y/include/uapi/asm-generic/errno-base.h \
  include/uapi/linux/kernel.h \
  /home/ccwang/bsp/git/nuc970/linux-3.10.y/include/uapi/linux/sysinfo.h \
  /home/ccwang/bsp/git/nuc970/linux-3.10.y/arch/arm/include/asm/div64.h \
  /home/ccwang/bsp/git/nuc970/linux-3.10.y/arch/arm/include/asm/compiler.h \
  /home/ccwang/bsp/git/nuc970/linux-3.10.y/arch/arm/include/asm/bug.h \
    $(wildcard include/config/bug.h) \
    $(wildcard include/config/thumb2/kernel.h) \
    $(wildcard include/config/debug/bugverbose.h) \
    $(wildcard include/config/arm/lpae.h) \
  include/asm-generic/bug.h \
    $(wildcard include/config/generic/bug/relative/pointers.h) \
  /home/ccwang/bsp/git/nuc970/linux-3.10.y/arch/arm/include/asm/cache.h \
    $(wildcard include/config/arm/l1/cache/shift.h) \
    $(wildcard include/config/aeabi.h) \
  include/linux/seqlock.h \
  include/linux/spinlock.h \
    $(wildcard include/config/debug/spinlock.h) \
    $(wildcard include/config/generic/lockbreak.h) \
    $(wildcard include/config/preempt.h) \
    $(wildcard include/config/debug/lock/alloc.h) \
  include/linux/preempt.h \
    $(wildcard include/config/debug/preempt.h) \
    $(wildcard include/config/context/tracking.h) \
    $(wildcard include/config/preempt/count.h) \
    $(wildcard include/config/preempt/notifiers.h) \
  include/linux/thread_info.h \
    $(wildcard include/config/compat.h) \
    $(wildcard include/config/debug/stack/usage.h) \
  include/linux/bug.h \
  /home/ccwang/bsp/git/nuc970/linux-3.10.y/arch/arm/include/asm/thread_info.h \
    $(wildcard include/config/crunch.h) \
    $(wildcard include/config/arm/thumbee.h) \
  /home/ccwang/bsp/git/nuc970/linux-3.10.y/arch/arm/include/asm/fpstate.h \
    $(wildcard include/config/vfpv3.h) \
    $(wildcard include/config/iwmmxt.h) \
  /home/ccwang/bsp/git/nuc970/linux-3.10.y/arch/arm/include/asm/domain.h \
    $(wildcard include/config/io/36.h) \
    $(wildcard include/config/cpu/use/domains.h) \
  /home/ccwang/bsp/git/nuc970/linux-3.10.y/arch/arm/include/asm/barrier.h \
    $(wildcard include/config/cpu/32v6k.h) \
    $(wildcard include/config/cpu/xsc3.h) \
    $(wildcard include/config/cpu/fa526.h) \
    $(wildcard include/config/arch/has/barriers.h) \
    $(wildcard include/config/arm/dma/mem/bufferable.h) \
  /home/ccwang/bsp/git/nuc970/linux-3.10.y/arch/arm/include/asm/outercache.h \
    $(wildcard include/config/outer/cache/sync.h) \
    $(wildcard include/config/outer/cache.h) \
  include/linux/bottom_half.h \
  include/linux/spinlock_types.h \
  include/linux/spinlock_types_up.h \
  include/linux/lockdep.h \
    $(wildcard include/config/lockdep.h) \
    $(wildcard include/config/lock/stat.h) \
    $(wildcard include/config/prove/rcu.h) \
  include/linux/rwlock_types.h \
  include/linux/spinlock_up.h \
  /home/ccwang/bsp/git/nuc970/linux-3.10.y/arch/arm/include/asm/processor.h \
    $(wildcard include/config/have/hw/breakpoint.h) \
    $(wildcard include/config/mmu.h) \
    $(wildcard include/config/arm/errata/754327.h) \
  /home/ccwang/bsp/git/nuc970/linux-3.10.y/arch/arm/include/asm/hw_breakpoint.h \
  include/linux/rwlock.h \
  include/linux/spinlock_api_up.h \
  include/linux/atomic.h \
    $(wildcard include/config/arch/has/atomic/or.h) \
    $(wildcard include/config/generic/atomic64.h) \
  /home/ccwang/bsp/git/nuc970/linux-3.10.y/arch/arm/include/asm/atomic.h \
  /home/ccwang/bsp/git/nuc970/linux-3.10.y/arch/arm/include/asm/cmpxchg.h \
    $(wildcard include/config/cpu/sa1100.h) \
    $(wildcard include/config/cpu/sa110.h) \
    $(wildcard include/config/cpu/v6.h) \
  include/asm-generic/cmpxchg-local.h \
  include/asm-generic/cmpxchg.h \
  include/asm-generic/atomic-long.h \
  include/asm-generic/atomic64.h \
  include/linux/math64.h \
  include/uapi/linux/time.h \
  include/linux/uidgid.h \
    $(wildcard include/config/uidgid/strict/type/checks.h) \
    $(wildcard include/config/user/ns.h) \
  include/linux/highuid.h \
  include/linux/kmod.h \
  include/linux/gfp.h \
    $(wildcard include/config/numa.h) \
    $(wildcard include/config/highmem.h) \
    $(wildcard include/config/zone/dma.h) \
    $(wildcard include/config/zone/dma32.h) \
    $(wildcard include/config/pm/sleep.h) \
    $(wildcard include/config/cma.h) \
  include/linux/mmzone.h \
    $(wildcard include/config/force/max/zoneorder.h) \
    $(wildcard include/config/memory/isolation.h) \
    $(wildcard include/config/memcg.h) \
    $(wildcard include/config/compaction.h) \
    $(wildcard include/config/memory/hotplug.h) \
    $(wildcard include/config/sparsemem.h) \
    $(wildcard include/config/have/memblock/node/map.h) \
    $(wildcard include/config/discontigmem.h) \
    $(wildcard include/config/flat/node/mem/map.h) \
    $(wildcard include/config/no/bootmem.h) \
    $(wildcard include/config/numa/balancing.h) \
    $(wildcard include/config/have/memory/present.h) \
    $(wildcard include/config/have/memoryless/nodes.h) \
    $(wildcard include/config/need/node/memmap/size.h) \
    $(wildcard include/config/need/multiple/nodes.h) \
    $(wildcard include/config/have/arch/early/pfn/to/nid.h) \
    $(wildcard include/config/flatmem.h) \
    $(wildcard include/config/sparsemem/extreme.h) \
    $(wildcard include/config/have/arch/pfn/valid.h) \
    $(wildcard include/config/nodes/span/other/nodes.h) \
    $(wildcard include/config/holes/in/zone.h) \
    $(wildcard include/config/arch/has/holes/memorymodel.h) \
  include/linux/wait.h \
  arch/arm/include/generated/asm/current.h \
  include/asm-generic/current.h \
  include/uapi/linux/wait.h \
  include/linux/threads.h \
    $(wildcard include/config/nr/cpus.h) \
    $(wildcard include/config/base/small.h) \
  include/linux/numa.h \
    $(wildcard include/config/nodes/shift.h) \
  include/linux/nodemask.h \
    $(wildcard include/config/movable/node.h) \
  include/linux/bitmap.h \
  include/linux/pageblock-flags.h \
    $(wildcard include/config/hugetlb/page.h) \
    $(wildcard include/config/hugetlb/page/size/variable.h) \
  include/linux/page-flags-layout.h \
    $(wildcard include/config/sparsemem/vmemmap.h) \
  include/generated/bounds.h \
  /home/ccwang/bsp/git/nuc970/linux-3.10.y/arch/arm/include/asm/page.h \
    $(wildcard include/config/cpu/copy/v4wt.h) \
    $(wildcard include/config/cpu/copy/v4wb.h) \
    $(wildcard include/config/cpu/copy/feroceon.h) \
    $(wildcard include/config/cpu/copy/fa.h) \
    $(wildcard include/config/cpu/xscale.h) \
    $(wildcard include/config/cpu/copy/v6.h) \
    $(wildcard include/config/kuser/helpers.h) \
  /home/ccwang/bsp/git/nuc970/linux-3.10.y/arch/arm/include/asm/glue.h \
  /home/ccwang/bsp/git/nuc970/linux-3.10.y/arch/arm/include/asm/pgtable-2level-types.h \
  /home/ccwang/bsp/git/nuc970/linux-3.10.y/arch/arm/include/asm/memory.h \
    $(wildcard include/config/need/mach/memory/h.h) \
    $(wildcard include/config/page/offset.h) \
    $(wildcard include/config/dram/size.h) \
    $(wildcard include/config/dram/base.h) \
    $(wildcard include/config/have/tcm.h) \
    $(wildcard include/config/arm/patch/phys/virt.h) \
    $(wildcard include/config/phys/offset.h) \
    $(wildcard include/config/virt/to/bus.h) \
  include/linux/sizes.h \
  include/asm-generic/memory_model.h \
  include/asm-generic/getorder.h \
  include/linux/memory_hotplug.h \
    $(wildcard include/config/memory/hotremove.h) \
    $(wildcard include/config/have/arch/nodedata/extension.h) \
    $(wildcard include/config/have/bootmem/info/node.h) \
  include/linux/notifier.h \
  include/linux/mutex.h \
    $(wildcard include/config/debug/mutexes.h) \
    $(wildcard include/config/mutex/spin/on/owner.h) \
    $(wildcard include/config/have/arch/mutex/cpu/relax.h) \
  include/linux/rwsem.h \
    $(wildcard include/config/rwsem/generic/spinlock.h) \
  include/linux/rwsem-spinlock.h \
  include/linux/srcu.h \
  include/linux/rcupdate.h \
    $(wildcard include/config/rcu/torture/test.h) \
    $(wildcard include/config/tree/rcu.h) \
    $(wildcard include/config/tree/preempt/rcu.h) \
    $(wildcard include/config/rcu/trace.h) \
    $(wildcard include/config/preempt/rcu.h) \
    $(wildcard include/config/rcu/user/qs.h) \
    $(wildcard include/config/tiny/rcu.h) \
    $(wildcard include/config/tiny/preempt/rcu.h) \
    $(wildcard include/config/debug/objects/rcu/head.h) \
    $(wildcard include/config/hotplug/cpu.h) \
    $(wildcard include/config/rcu/nocb/cpu.h) \
  include/linux/cpumask.h \
    $(wildcard include/config/cpumask/offstack.h) \
    $(wildcard include/config/debug/per/cpu/maps.h) \
    $(wildcard include/config/disable/obsolete/cpumask/functions.h) \
  include/linux/completion.h \
  include/linux/debugobjects.h \
    $(wildcard include/config/debug/objects.h) \
    $(wildcard include/config/debug/objects/free.h) \
  include/linux/rcutree.h \
  include/linux/workqueue.h \
    $(wildcard include/config/debug/objects/work.h) \
    $(wildcard include/config/freezer.h) \
  include/linux/timer.h \
    $(wildcard include/config/timer/stats.h) \
    $(wildcard include/config/debug/objects/timers.h) \
  include/linux/ktime.h \
    $(wildcard include/config/ktime/scalar.h) \
  include/linux/jiffies.h \
  include/linux/timex.h \
  include/uapi/linux/timex.h \
  /home/ccwang/bsp/git/nuc970/linux-3.10.y/include/uapi/linux/param.h \
  arch/arm/include/generated/asm/param.h \
  include/asm-generic/param.h \
    $(wildcard include/config/hz.h) \
  include/uapi/asm-generic/param.h \
  /home/ccwang/bsp/git/nuc970/linux-3.10.y/arch/arm/include/asm/timex.h \
    $(wildcard include/config/arch/multiplatform.h) \
  arch/arm/mach-nuc970/include/mach/timex.h \
  include/linux/topology.h \
    $(wildcard include/config/sched/smt.h) \
    $(wildcard include/config/sched/mc.h) \
    $(wildcard include/config/sched/book.h) \
    $(wildcard include/config/use/percpu/numa/node/id.h) \
  include/linux/smp.h \
    $(wildcard include/config/use/generic/smp/helpers.h) \
  include/linux/percpu.h \
    $(wildcard include/config/need/per/cpu/embed/first/chunk.h) \
    $(wildcard include/config/need/per/cpu/page/first/chunk.h) \
    $(wildcard include/config/have/setup/per/cpu/area.h) \
  include/linux/pfn.h \
  /home/ccwang/bsp/git/nuc970/linux-3.10.y/arch/arm/include/asm/percpu.h \
  include/asm-generic/percpu.h \
  include/linux/percpu-defs.h \
    $(wildcard include/config/debug/force/weak/per/cpu.h) \
  /home/ccwang/bsp/git/nuc970/linux-3.10.y/arch/arm/include/asm/topology.h \
    $(wildcard include/config/arm/cpu/topology.h) \
  include/asm-generic/topology.h \
  include/linux/mmdebug.h \
    $(wildcard include/config/debug/vm.h) \
    $(wildcard include/config/debug/virtual.h) \
  include/linux/sysctl.h \
    $(wildcard include/config/sysctl.h) \
  include/linux/rbtree.h \
  include/uapi/linux/sysctl.h \
  include/linux/elf.h \
  /home/ccwang/bsp/git/nuc970/linux-3.10.y/arch/arm/include/asm/elf.h \
  /home/ccwang/bsp/git/nuc970/linux-3.10.y/arch/arm/include/asm/user.h \
  include/uapi/linux/elf.h \
  /home/ccwang/bsp/git/nuc970/linux-3.10.y/include/uapi/linux/elf-em.h \
  include/linux/kobject.h \
  include/linux/sysfs.h \
  include/linux/kobject_ns.h \
  include/linux/kref.h \
  include/linux/moduleparam.h \
    $(wildcard include/config/alpha.h) \
    $(wildcard include/config/ia64.h) \
    $(wildcard include/config/ppc64.h) \
  include/linux/tracepoint.h \
  include/linux/static_key.h \
  include/linux/jump_label.h \
    $(wildcard include/config/jump/label.h) \
  /home/ccwang/bsp/git/nuc970/linux-3.10.y/arch/arm/include/asm/module.h \
    $(wildcard include/config/arm/unwind.h) \
  include/asm-generic/module.h \
    $(wildcard include/config/have/mod/arch/specific.h) \
    $(wildcard include/config/modules/use/elf/rel.h) \
    $(wildcard include/config/modules/use/elf/rela.h) \
  include/generated/uapi/linux/version.h \
  include/linux/slab.h \
    $(wildcard include/config/slab/debug.h) \
    $(wildcard include/config/kmemcheck.h) \
    $(wildcard include/config/failslab.h) \
    $(wildcard include/config/slob.h) \
    $(wildcard include/config/slab.h) \
    $(wildcard include/config/slub.h) \
    $(wildcard include/config/debug/slab.h) \
  include/linux/slub_def.h \
    $(wildcard include/config/slub/stats.h) \
    $(wildcard include/config/memcg/kmem.h) \
    $(wildcard include/config/slub/debug.h) \
  include/linux/kmemleak.h \
    $(wildcard include/config/debug/kmemleak.h) \
  include/linux/interrupt.h \
    $(wildcard include/config/generic/hardirqs.h) \
    $(wildcard include/config/irq/forced/threading.h) \
    $(wildcard include/config/generic/irq/probe.h) \
    $(wildcard include/config/proc/fs.h) \
  include/linux/irqreturn.h \
  include/linux/irqnr.h \
  include/uapi/linux/irqnr.h \
  include/linux/hardirq.h \
  include/linux/ftrace_irq.h \
    $(wildcard include/config/ftrace/nmi/enter.h) \
  include/linux/vtime.h \
    $(wildcard include/config/virt/cpu/accounting.h) \
    $(wildcard include/config/virt/cpu/accounting/native.h) \
    $(wildcard include/config/virt/cpu/accounting/gen.h) \
    $(wildcard include/config/irq/time/accounting.h) \
  /home/ccwang/bsp/git/nuc970/linux-3.10.y/arch/arm/include/asm/hardirq.h \
  /home/ccwang/bsp/git/nuc970/linux-3.10.y/arch/arm/include/asm/irq.h \
    $(wildcard include/config/sparse/irq.h) \
    $(wildcard include/config/multi/irq/handler.h) \
  arch/arm/mach-nuc970/include/mach/irqs.h \
  include/linux/irq_cpustat.h \
  include/linux/hrtimer.h \
    $(wildcard include/config/high/res/timers.h) \
    $(wildcard include/config/timerfd.h) \
  include/linux/timerqueue.h \
  include/linux/pci.h \
    $(wildcard include/config/pci/iov.h) \
    $(wildcard include/config/pcieaspm.h) \
    $(wildcard include/config/pci/msi.h) \
    $(wildcard include/config/pci/ats.h) \
    $(wildcard include/config/pci.h) \
    $(wildcard include/config/pcieportbus.h) \
    $(wildcard include/config/pcieaer.h) \
    $(wildcard include/config/pcie/ecrc.h) \
    $(wildcard include/config/ht/irq.h) \
    $(wildcard include/config/pci/domains.h) \
    $(wildcard include/config/pci/quirks.h) \
    $(wildcard include/config/pci/mmconfig.h) \
    $(wildcard include/config/hotplug/pci.h) \
    $(wildcard include/config/of.h) \
    $(wildcard include/config/eeh.h) \
  include/linux/mod_devicetable.h \
  include/linux/uuid.h \
  include/uapi/linux/uuid.h \
  include/linux/ioport.h \
  include/linux/device.h \
    $(wildcard include/config/debug/devres.h) \
    $(wildcard include/config/acpi.h) \
    $(wildcard include/config/pinctrl.h) \
    $(wildcard include/config/devtmpfs.h) \
    $(wildcard include/config/sysfs/deprecated.h) \
  include/linux/klist.h \
  include/linux/pinctrl/devinfo.h \
  include/linux/pm.h \
    $(wildcard include/config/vt/console/sleep.h) \
    $(wildcard include/config/pm.h) \
    $(wildcard include/config/pm/runtime.h) \
    $(wildcard include/config/pm/clk.h) \
    $(wildcard include/config/pm/generic/domains.h) \
  include/linux/ratelimit.h \
  /home/ccwang/bsp/git/nuc970/linux-3.10.y/arch/arm/include/asm/device.h \
    $(wildcard include/config/dmabounce.h) \
    $(wildcard include/config/iommu/api.h) \
    $(wildcard include/config/arm/dma/use/iommu.h) \
    $(wildcard include/config/arch/omap.h) \
  include/linux/pm_wakeup.h \
  include/linux/io.h \
    $(wildcard include/config/has/ioport.h) \
  /home/ccwang/bsp/git/nuc970/linux-3.10.y/arch/arm/include/asm/io.h \
    $(wildcard include/config/need/mach/io/h.h) \
    $(wildcard include/config/pcmcia/soc/common.h) \
    $(wildcard include/config/isa.h) \
    $(wildcard include/config/pccard.h) \
  include/asm-generic/pci_iomap.h \
    $(wildcard include/config/no/generic/pci/ioport/map.h) \
    $(wildcard include/config/generic/pci/iomap.h) \
  include/uapi/linux/pci.h \
  /home/ccwang/bsp/git/nuc970/linux-3.10.y/include/uapi/linux/pci_regs.h \
  include/linux/pci_ids.h \
  /home/ccwang/bsp/git/nuc970/linux-3.10.y/arch/arm/include/asm/pci.h \
  include/asm-generic/pci-dma-compat.h \
  include/linux/dma-mapping.h \
    $(wildcard include/config/has/dma.h) \
    $(wildcard include/config/arch/has/dma/set/coherent/mask.h) \
    $(wildcard include/config/have/dma/attrs.h) \
    $(wildcard include/config/need/dma/map/state.h) \
  include/linux/err.h \
  include/linux/dma-attrs.h \
  include/linux/dma-direction.h \
  include/linux/scatterlist.h \
    $(wildcard include/config/debug/sg.h) \
  include/linux/mm.h \
    $(wildcard include/config/x86.h) \
    $(wildcard include/config/ppc.h) \
    $(wildcard include/config/parisc.h) \
    $(wildcard include/config/metag.h) \
    $(wildcard include/config/stack/growsup.h) \
    $(wildcard include/config/transparent/hugepage.h) \
    $(wildcard include/config/ksm.h) \
    $(wildcard include/config/debug/vm/rb.h) \
    $(wildcard include/config/arch/uses/numa/prot/none.h) \
    $(wildcard include/config/debug/pagealloc.h) \
    $(wildcard include/config/hibernation.h) \
    $(wildcard include/config/hugetlbfs.h) \
  include/linux/debug_locks.h \
    $(wildcard include/config/debug/locking/api/selftests.h) \
  include/linux/mm_types.h \
    $(wildcard include/config/split/ptlock/cpus.h) \
    $(wildcard include/config/have/cmpxchg/double.h) \
    $(wildcard include/config/have/aligned/struct/page.h) \
    $(wildcard include/config/want/page/debug/flags.h) \
    $(wildcard include/config/aio.h) \
    $(wildcard include/config/mm/owner.h) \
    $(wildcard include/config/mmu/notifier.h) \
  include/linux/auxvec.h \
  include/uapi/linux/auxvec.h \
  arch/arm/include/generated/asm/auxvec.h \
  /home/ccwang/bsp/git/nuc970/linux-3.10.y/include/uapi/asm-generic/auxvec.h \
  include/linux/page-debug-flags.h \
    $(wildcard include/config/page/poisoning.h) \
    $(wildcard include/config/page/guard.h) \
    $(wildcard include/config/page/debug/something/else.h) \
  include/linux/uprobes.h \
    $(wildcard include/config/arch/supports/uprobes.h) \
    $(wildcard include/config/uprobes.h) \
  /home/ccwang/bsp/git/nuc970/linux-3.10.y/arch/arm/include/asm/mmu.h \
    $(wildcard include/config/cpu/has/asid.h) \
  include/linux/range.h \
  include/linux/bit_spinlock.h \
  include/linux/shrinker.h \
  /home/ccwang/bsp/git/nuc970/linux-3.10.y/arch/arm/include/asm/pgtable.h \
    $(wildcard include/config/highpte.h) \
  /home/ccwang/bsp/git/nuc970/linux-3.10.y/arch/arm/include/asm/proc-fns.h \
  /home/ccwang/bsp/git/nuc970/linux-3.10.y/arch/arm/include/asm/glue-proc.h \
    $(wildcard include/config/cpu/arm7tdmi.h) \
    $(wildcard include/config/cpu/arm720t.h) \
    $(wildcard include/config/cpu/arm740t.h) \
    $(wildcard include/config/cpu/arm9tdmi.h) \
    $(wildcard include/config/cpu/arm920t.h) \
    $(wildcard include/config/cpu/arm922t.h) \
    $(wildcard include/config/cpu/arm925t.h) \
    $(wildcard include/config/cpu/arm926t.h) \
    $(wildcard include/config/cpu/arm940t.h) \
    $(wildcard include/config/cpu/arm946e.h) \
    $(wildcard include/config/cpu/arm1020.h) \
    $(wildcard include/config/cpu/arm1020e.h) \
    $(wildcard include/config/cpu/arm1022.h) \
    $(wildcard include/config/cpu/arm1026.h) \
    $(wildcard include/config/cpu/mohawk.h) \
    $(wildcard include/config/cpu/feroceon.h) \
    $(wildcard include/config/cpu/v6k.h) \
    $(wildcard include/config/cpu/v7.h) \
    $(wildcard include/config/cpu/pj4b.h) \
  include/asm-generic/pgtable-nopud.h \
  /home/ccwang/bsp/git/nuc970/linux-3.10.y/arch/arm/include/asm/pgtable-hwdef.h \
  /home/ccwang/bsp/git/nuc970/linux-3.10.y/arch/arm/include/asm/pgtable-2level-hwdef.h \
  /home/ccwang/bsp/git/nuc970/linux-3.10.y/arch/arm/include/asm/pgtable-2level.h \
  include/asm-generic/pgtable.h \
  include/linux/page-flags.h \
    $(wildcard include/config/pageflags/extended.h) \
    $(wildcard include/config/arch/uses/pg/uncached.h) \
    $(wildcard include/config/memory/failure.h) \
    $(wildcard include/config/swap.h) \
  include/linux/huge_mm.h \
  include/linux/vmstat.h \
    $(wildcard include/config/vm/event/counters.h) \
  include/linux/vm_event_item.h \
    $(wildcard include/config/migration.h) \
  /home/ccwang/bsp/git/nuc970/linux-3.10.y/arch/arm/include/asm/scatterlist.h \
    $(wildcard include/config/arm/has/sg/chain.h) \
  include/asm-generic/scatterlist.h \
    $(wildcard include/config/need/sg/dma/length.h) \
  /home/ccwang/bsp/git/nuc970/linux-3.10.y/arch/arm/include/asm/dma-mapping.h \
  include/linux/dma-debug.h \
    $(wildcard include/config/dma/api/debug.h) \
  include/asm-generic/dma-coherent.h \
    $(wildcard include/config/have/generic/dma/coherent.h) \
  include/asm-generic/dma-mapping-common.h \
  include/linux/kmemcheck.h \
  include/asm-generic/pci-bridge.h \
  /home/ccwang/bsp/git/nuc970/linux-3.10.y/arch/arm/include/asm/mach/pci.h \
  include/linux/netdevice.h \
    $(wildcard include/config/dcb.h) \
    $(wildcard include/config/wlan.h) \
    $(wildcard include/config/ax25.h) \
    $(wildcard include/config/mac80211/mesh.h) \
    $(wildcard include/config/net/ipip.h) \
    $(wildcard include/config/net/ipgre.h) \
    $(wildcard include/config/ipv6/sit.h) \
    $(wildcard include/config/ipv6/tunnel.h) \
    $(wildcard include/config/rps.h) \
    $(wildcard include/config/netpoll.h) \
    $(wildcard include/config/xps.h) \
    $(wildcard include/config/bql.h) \
    $(wildcard include/config/rfs/accel.h) \
    $(wildcard include/config/fcoe.h) \
    $(wildcard include/config/net/poll/controller.h) \
    $(wildcard include/config/libfcoe.h) \
    $(wildcard include/config/wireless/ext.h) \
    $(wildcard include/config/vlan/8021q.h) \
    $(wildcard include/config/net/dsa.h) \
    $(wildcard include/config/net/ns.h) \
    $(wildcard include/config/netprio/cgroup.h) \
    $(wildcard include/config/net/dsa/tag/dsa.h) \
    $(wildcard include/config/net/dsa/tag/trailer.h) \
    $(wildcard include/config/netpoll/trap.h) \
  include/linux/pm_qos.h \
  include/linux/plist.h \
    $(wildcard include/config/debug/pi/list.h) \
  include/linux/miscdevice.h \
  /home/ccwang/bsp/git/nuc970/linux-3.10.y/include/uapi/linux/major.h \
  include/linux/delay.h \
  /home/ccwang/bsp/git/nuc970/linux-3.10.y/arch/arm/include/asm/delay.h \
  include/linux/rculist.h \
  include/linux/dmaengine.h \
    $(wildcard include/config/async/tx/enable/channel/switch.h) \
    $(wildcard include/config/rapidio/dma/engine.h) \
    $(wildcard include/config/dma/engine.h) \
    $(wildcard include/config/net/dma.h) \
    $(wildcard include/config/async/tx/dma.h) \
  include/linux/uio.h \
  include/uapi/linux/uio.h \
  include/linux/dynamic_queue_limits.h \
  include/linux/ethtool.h \
  include/linux/compat.h \
    $(wildcard include/config/compat/old/sigaction.h) \
    $(wildcard include/config/odd/rt/sigaction.h) \
  include/uapi/linux/ethtool.h \
  include/linux/if_ether.h \
  include/linux/skbuff.h \
    $(wildcard include/config/nf/conntrack.h) \
    $(wildcard include/config/bridge/netfilter.h) \
    $(wildcard include/config/xfrm.h) \
    $(wildcard include/config/net/sched.h) \
    $(wildcard include/config/net/cls/act.h) \
    $(wildcard include/config/ipv6/ndisc/nodetype.h) \
    $(wildcard include/config/network/secmark.h) \
    $(wildcard include/config/network/phy/timestamping.h) \
    $(wildcard include/config/netfilter/xt/target/trace.h) \
  include/linux/net.h \
  include/linux/random.h \
    $(wildcard include/config/arch/random.h) \
  include/uapi/linux/random.h \
  /home/ccwang/bsp/git/nuc970/linux-3.10.y/include/uapi/linux/ioctl.h \
  arch/arm/include/generated/asm/ioctl.h \
  include/asm-generic/ioctl.h \
  include/uapi/asm-generic/ioctl.h \
  include/linux/fcntl.h \
  include/uapi/linux/fcntl.h \
  /home/ccwang/bsp/git/nuc970/linux-3.10.y/arch/arm/include/uapi/asm/fcntl.h \
  /home/ccwang/bsp/git/nuc970/linux-3.10.y/include/uapi/asm-generic/fcntl.h \
  include/uapi/linux/net.h \
  include/linux/socket.h \
  arch/arm/include/generated/asm/socket.h \
  /home/ccwang/bsp/git/nuc970/linux-3.10.y/include/uapi/asm-generic/socket.h \
  arch/arm/include/generated/asm/sockios.h \
  /home/ccwang/bsp/git/nuc970/linux-3.10.y/include/uapi/asm-generic/sockios.h \
  /home/ccwang/bsp/git/nuc970/linux-3.10.y/include/uapi/linux/sockios.h \
  include/uapi/linux/socket.h \
  include/linux/textsearch.h \
  include/net/checksum.h \
  /home/ccwang/bsp/git/nuc970/linux-3.10.y/arch/arm/include/asm/uaccess.h \
  /home/ccwang/bsp/git/nuc970/linux-3.10.y/arch/arm/include/asm/unified.h \
    $(wildcard include/config/arm/asm/unified.h) \
  /home/ccwang/bsp/git/nuc970/linux-3.10.y/arch/arm/include/asm/checksum.h \
  include/linux/in6.h \
  include/uapi/linux/in6.h \
  include/linux/netdev_features.h \
  include/net/flow_keys.h \
  include/uapi/linux/if_ether.h \
  include/net/net_namespace.h \
    $(wildcard include/config/ipv6.h) \
    $(wildcard include/config/ip/sctp.h) \
    $(wildcard include/config/ip/dccp.h) \
    $(wildcard include/config/netfilter.h) \
    $(wildcard include/config/nf/defrag/ipv6.h) \
    $(wildcard include/config/wext/core.h) \
  include/net/netns/core.h \
  include/net/netns/mib.h \
    $(wildcard include/config/xfrm/statistics.h) \
  include/net/snmp.h \
  /home/ccwang/bsp/git/nuc970/linux-3.10.y/include/uapi/linux/snmp.h \
  include/linux/u64_stats_sync.h \
  include/net/netns/unix.h \
  include/net/netns/packet.h \
  include/net/netns/ipv4.h \
    $(wildcard include/config/ip/multiple/tables.h) \
    $(wildcard include/config/ip/route/classid.h) \
    $(wildcard include/config/security.h) \
    $(wildcard include/config/ip/mroute.h) \
    $(wildcard include/config/ip/mroute/multiple/tables.h) \
  include/net/inet_frag.h \
  include/linux/percpu_counter.h \
  include/net/netns/ipv6.h \
    $(wildcard include/config/ipv6/multiple/tables.h) \
    $(wildcard include/config/ipv6/mroute.h) \
    $(wildcard include/config/ipv6/mroute/multiple/tables.h) \
  include/net/dst_ops.h \
  include/net/netns/sctp.h \
  include/net/netns/dccp.h \
  include/net/netns/netfilter.h \
  include/linux/proc_fs.h \
  include/linux/fs.h \
    $(wildcard include/config/fs/posix/acl.h) \
    $(wildcard include/config/quota.h) \
    $(wildcard include/config/fsnotify.h) \
    $(wildcard include/config/ima.h) \
    $(wildcard include/config/epoll.h) \
    $(wildcard include/config/debug/writecount.h) \
    $(wildcard include/config/file/locking.h) \
    $(wildcard include/config/auditsyscall.h) \
    $(wildcard include/config/block.h) \
    $(wildcard include/config/fs/xip.h) \
  include/linux/kdev_t.h \
  include/uapi/linux/kdev_t.h \
  include/linux/dcache.h \
  include/linux/rculist_bl.h \
  include/linux/list_bl.h \
  include/linux/path.h \
  include/linux/radix-tree.h \
  include/linux/pid.h \
  include/linux/capability.h \
  include/uapi/linux/capability.h \
  include/linux/semaphore.h \
  /home/ccwang/bsp/git/nuc970/linux-3.10.y/include/uapi/linux/fiemap.h \
  include/linux/migrate_mode.h \
  include/linux/percpu-rwsem.h \
  include/linux/blk_types.h \
    $(wildcard include/config/blk/cgroup.h) \
    $(wildcard include/config/blk/dev/integrity.h) \
  include/uapi/linux/fs.h \
  /home/ccwang/bsp/git/nuc970/linux-3.10.y/include/uapi/linux/limits.h \
  include/linux/quota.h \
    $(wildcard include/config/quota/netlink/interface.h) \
  /home/ccwang/bsp/git/nuc970/linux-3.10.y/include/uapi/linux/dqblk_xfs.h \
  include/linux/dqblk_v1.h \
  include/linux/dqblk_v2.h \
  include/linux/dqblk_qtree.h \
  include/linux/projid.h \
  include/uapi/linux/quota.h \
  include/linux/nfs_fs_i.h \
  include/linux/netfilter.h \
    $(wildcard include/config/nf/nat/needed.h) \
  /home/ccwang/bsp/git/nuc970/linux-3.10.y/include/uapi/linux/if.h \
  /home/ccwang/bsp/git/nuc970/linux-3.10.y/include/uapi/linux/hdlc/ioctl.h \
  include/linux/in.h \
  include/uapi/linux/in.h \
  include/uapi/linux/netfilter.h \
  include/net/netns/x_tables.h \
    $(wildcard include/config/bridge/nf/ebtables.h) \
  include/net/netns/xfrm.h \
  /home/ccwang/bsp/git/nuc970/linux-3.10.y/include/uapi/linux/xfrm.h \
  include/linux/seq_file_net.h \
  include/linux/seq_file.h \
  include/net/dsa.h \
  include/net/netprio_cgroup.h \
  include/linux/cgroup.h \
    $(wildcard include/config/cgroups.h) \
  include/linux/sched.h \
    $(wildcard include/config/sched/debug.h) \
    $(wildcard include/config/no/hz/common.h) \
    $(wildcard include/config/lockup/detector.h) \
    $(wildcard include/config/core/dump/default/elf/headers.h) \
    $(wildcard include/config/sched/autogroup.h) \
    $(wildcard include/config/bsd/process/acct.h) \
    $(wildcard include/config/taskstats.h) \
    $(wildcard include/config/audit.h) \
    $(wildcard include/config/inotify/user.h) \
    $(wildcard include/config/fanotify.h) \
    $(wildcard include/config/posix/mqueue.h) \
    $(wildcard include/config/keys.h) \
    $(wildcard include/config/perf/events.h) \
    $(wildcard include/config/schedstats.h) \
    $(wildcard include/config/task/delay/acct.h) \
    $(wildcard include/config/fair/group/sched.h) \
    $(wildcard include/config/rt/group/sched.h) \
    $(wildcard include/config/cgroup/sched.h) \
    $(wildcard include/config/blk/dev/io/trace.h) \
    $(wildcard include/config/rcu/boost.h) \
    $(wildcard include/config/compat/brk.h) \
    $(wildcard include/config/cc/stackprotector.h) \
    $(wildcard include/config/sysvipc.h) \
    $(wildcard include/config/detect/hung/task.h) \
    $(wildcard include/config/rt/mutexes.h) \
    $(wildcard include/config/task/xacct.h) \
    $(wildcard include/config/cpusets.h) \
    $(wildcard include/config/futex.h) \
    $(wildcard include/config/fault/injection.h) \
    $(wildcard include/config/latencytop.h) \
    $(wildcard include/config/function/graph/tracer.h) \
    $(wildcard include/config/bcache.h) \
    $(wildcard include/config/have/unstable/sched/clock.h) \
    $(wildcard include/config/no/hz/full.h) \
  include/uapi/linux/sched.h \
  arch/arm/include/generated/asm/cputime.h \
  include/asm-generic/cputime.h \
  include/asm-generic/cputime_jiffies.h \
  include/linux/sem.h \
  include/uapi/linux/sem.h \
  include/linux/ipc.h \
  include/uapi/linux/ipc.h \
  arch/arm/include/generated/asm/ipcbuf.h \
  /home/ccwang/bsp/git/nuc970/linux-3.10.y/include/uapi/asm-generic/ipcbuf.h \
  arch/arm/include/generated/asm/sembuf.h \
  /home/ccwang/bsp/git/nuc970/linux-3.10.y/include/uapi/asm-generic/sembuf.h \
  include/linux/signal.h \
    $(wildcard include/config/old/sigaction.h) \
  include/uapi/linux/signal.h \
  /home/ccwang/bsp/git/nuc970/linux-3.10.y/arch/arm/include/asm/signal.h \
  /home/ccwang/bsp/git/nuc970/linux-3.10.y/arch/arm/include/uapi/asm/signal.h \
  /home/ccwang/bsp/git/nuc970/linux-3.10.y/include/uapi/asm-generic/signal-defs.h \
  /home/ccwang/bsp/git/nuc970/linux-3.10.y/arch/arm/include/uapi/asm/sigcontext.h \
  arch/arm/include/generated/asm/siginfo.h \
  include/asm-generic/siginfo.h \
  include/uapi/asm-generic/siginfo.h \
  include/linux/proportions.h \
  include/linux/seccomp.h \
    $(wildcard include/config/seccomp.h) \
    $(wildcard include/config/seccomp/filter.h) \
  include/uapi/linux/seccomp.h \
  include/linux/rtmutex.h \
    $(wildcard include/config/debug/rt/mutexes.h) \
  include/linux/resource.h \
  include/uapi/linux/resource.h \
  arch/arm/include/generated/asm/resource.h \
  include/asm-generic/resource.h \
  include/uapi/asm-generic/resource.h \
  include/linux/task_io_accounting.h \
    $(wildcard include/config/task/io/accounting.h) \
  include/linux/latencytop.h \
  include/linux/cred.h \
    $(wildcard include/config/debug/credentials.h) \
  include/linux/key.h \
  include/linux/selinux.h \
    $(wildcard include/config/security/selinux.h) \
  include/linux/llist.h \
    $(wildcard include/config/arch/have/nmi/safe/cmpxchg.h) \
  /home/ccwang/bsp/git/nuc970/linux-3.10.y/include/uapi/linux/cgroupstats.h \
  /home/ccwang/bsp/git/nuc970/linux-3.10.y/include/uapi/linux/taskstats.h \
  include/linux/prio_heap.h \
  include/linux/idr.h \
  include/linux/xattr.h \
  include/uapi/linux/xattr.h \
  include/linux/cgroup_subsys.h \
    $(wildcard include/config/cgroup/debug.h) \
    $(wildcard include/config/cgroup/cpuacct.h) \
    $(wildcard include/config/cgroup/device.h) \
    $(wildcard include/config/cgroup/freezer.h) \
    $(wildcard include/config/net/cls/cgroup.h) \
    $(wildcard include/config/cgroup/perf.h) \
    $(wildcard include/config/cgroup/hugetlb.h) \
    $(wildcard include/config/cgroup/bcache.h) \
  /home/ccwang/bsp/git/nuc970/linux-3.10.y/include/uapi/linux/neighbour.h \
  include/linux/netlink.h \
  include/net/scm.h \
    $(wildcard include/config/security/network.h) \
  include/linux/security.h \
    $(wildcard include/config/security/path.h) \
    $(wildcard include/config/security/network/xfrm.h) \
    $(wildcard include/config/securityfs.h) \
    $(wildcard include/config/security/yama.h) \
  include/linux/nsproxy.h \
  include/uapi/linux/netlink.h \
  include/uapi/linux/netdevice.h \
  /home/ccwang/bsp/git/nuc970/linux-3.10.y/include/uapi/linux/if_packet.h \
  include/linux/if_link.h \
  include/uapi/linux/if_link.h \
  include/linux/etherdevice.h \
    $(wildcard include/config/have/efficient/unaligned/access.h) \
  arch/arm/include/generated/asm/unaligned.h \
  include/asm-generic/unaligned.h \
  include/linux/unaligned/le_struct.h \
  include/linux/unaligned/packed_struct.h \
  include/linux/unaligned/be_byteshift.h \
  include/linux/unaligned/generic.h \
  include/linux/wireless.h \
  include/uapi/linux/wireless.h \
  include/linux/if_arp.h \
    $(wildcard include/config/firewire/net.h) \
  include/uapi/linux/if_arp.h \
  include/linux/ctype.h \
  include/linux/vmalloc.h \
  include/linux/usb.h \
    $(wildcard include/config/usb/mon.h) \
  include/linux/usb/ch9.h \
  include/uapi/linux/usb/ch9.h \
    $(wildcard include/config/size.h) \
    $(wildcard include/config/att/one.h) \
    $(wildcard include/config/att/selfpower.h) \
    $(wildcard include/config/att/wakeup.h) \
    $(wildcard include/config/att/battery.h) \
  include/linux/pm_runtime.h \
  include/net/iw_handler.h \
    $(wildcard include/config/wext/priv.h) \
  /home/ccwang/bsp/git/nuc970/linux-3.10.y/include/uapi/linux/unistd.h \
  /home/ccwang/bsp/git/nuc970/linux-3.10.y/arch/arm/include/asm/unistd.h \
    $(wildcard include/config/oabi/compat.h) \
  /home/ccwang/bsp/git/nuc970/linux-3.10.y/arch/arm/include/uapi/asm/unistd.h \
  include/linux/kthread.h \
  /home/ccwang/bsp/git/nuc970/apps/DPO_RT3070_LinuxSTA_V2.3.0.2_20100412/include/rtmp_def.h \
  /home/ccwang/bsp/git/nuc970/apps/DPO_RT3070_LinuxSTA_V2.3.0.2_20100412/include/oid.h \
    $(wildcard include/config/status.h) \
  /home/ccwang/bsp/git/nuc970/apps/DPO_RT3070_LinuxSTA_V2.3.0.2_20100412/include/rtmp_chip.h \
  /home/ccwang/bsp/git/nuc970/apps/DPO_RT3070_LinuxSTA_V2.3.0.2_20100412/include/chip/rt3070.h \
  /home/ccwang/bsp/git/nuc970/apps/DPO_RT3070_LinuxSTA_V2.3.0.2_20100412/include/chip/mac_usb.h \
  /home/ccwang/bsp/git/nuc970/apps/DPO_RT3070_LinuxSTA_V2.3.0.2_20100412/include/rtmp_type.h \
  /home/ccwang/bsp/git/nuc970/apps/DPO_RT3070_LinuxSTA_V2.3.0.2_20100412/include/chip/rtmp_mac.h \
  /home/ccwang/bsp/git/nuc970/apps/DPO_RT3070_LinuxSTA_V2.3.0.2_20100412/include/chip/rtmp_phy.h \
  /home/ccwang/bsp/git/nuc970/apps/DPO_RT3070_LinuxSTA_V2.3.0.2_20100412/include/rtmp_iface.h \
    $(wildcard include/config/.h) \
  /home/ccwang/bsp/git/nuc970/apps/DPO_RT3070_LinuxSTA_V2.3.0.2_20100412/include/iface/rtmp_usb.h \
  /home/ccwang/bsp/git/nuc970/apps/DPO_RT3070_LinuxSTA_V2.3.0.2_20100412/include/rtusb_io.h \
  /home/ccwang/bsp/git/nuc970/apps/DPO_RT3070_LinuxSTA_V2.3.0.2_20100412/include/wpa_cmm.h \
  /home/ccwang/bsp/git/nuc970/apps/DPO_RT3070_LinuxSTA_V2.3.0.2_20100412/include/dot11i_wpa.h \
  /home/ccwang/bsp/git/nuc970/apps/DPO_RT3070_LinuxSTA_V2.3.0.2_20100412/include/rtmp_dot11.h \
  /home/ccwang/bsp/git/nuc970/apps/DPO_RT3070_LinuxSTA_V2.3.0.2_20100412/include/chip/rt30xx.h \
  /home/ccwang/bsp/git/nuc970/apps/DPO_RT3070_LinuxSTA_V2.3.0.2_20100412/include/rtmp_timer.h \
  /home/ccwang/bsp/git/nuc970/apps/DPO_RT3070_LinuxSTA_V2.3.0.2_20100412/include/mlme.h \
  /home/ccwang/bsp/git/nuc970/apps/DPO_RT3070_LinuxSTA_V2.3.0.2_20100412/include/rtmp_dot11.h \
  /home/ccwang/bsp/git/nuc970/apps/DPO_RT3070_LinuxSTA_V2.3.0.2_20100412/include/crypt_md5.h \
  /home/ccwang/bsp/git/nuc970/apps/DPO_RT3070_LinuxSTA_V2.3.0.2_20100412/include/crypt_sha2.h \
  /home/ccwang/bsp/git/nuc970/apps/DPO_RT3070_LinuxSTA_V2.3.0.2_20100412/include/crypt_hmac.h \
  /home/ccwang/bsp/git/nuc970/apps/DPO_RT3070_LinuxSTA_V2.3.0.2_20100412/include/rt_config.h \
  /home/ccwang/bsp/git/nuc970/apps/DPO_RT3070_LinuxSTA_V2.3.0.2_20100412/include/crypt_aes.h \
  /home/ccwang/bsp/git/nuc970/apps/DPO_RT3070_LinuxSTA_V2.3.0.2_20100412/include/crypt_arc4.h \
  /home/ccwang/bsp/git/nuc970/apps/DPO_RT3070_LinuxSTA_V2.3.0.2_20100412/include/rtmp_cmd.h \
  /home/ccwang/bsp/git/nuc970/apps/DPO_RT3070_LinuxSTA_V2.3.0.2_20100412/include/rtmp.h \
    $(wildcard include/config/ap/support.h) \
    $(wildcard include/config/opmode/on/ap.h) \
    $(wildcard include/config/opmode/on/sta.h) \
  /home/ccwang/bsp/git/nuc970/apps/DPO_RT3070_LinuxSTA_V2.3.0.2_20100412/include/link_list.h \
  /home/ccwang/bsp/git/nuc970/apps/DPO_RT3070_LinuxSTA_V2.3.0.2_20100412/include/spectrum_def.h \
  /home/ccwang/bsp/git/nuc970/apps/DPO_RT3070_LinuxSTA_V2.3.0.2_20100412/include/ap.h \
  /home/ccwang/bsp/git/nuc970/apps/DPO_RT3070_LinuxSTA_V2.3.0.2_20100412/include/wpa.h \
  /home/ccwang/bsp/git/nuc970/apps/DPO_RT3070_LinuxSTA_V2.3.0.2_20100412/include/dfs.h \
  /home/ccwang/bsp/git/nuc970/apps/DPO_RT3070_LinuxSTA_V2.3.0.2_20100412/include/chlist.h \
  /home/ccwang/bsp/git/nuc970/apps/DPO_RT3070_LinuxSTA_V2.3.0.2_20100412/include/spectrum.h \
  /home/ccwang/bsp/git/nuc970/apps/DPO_RT3070_LinuxSTA_V2.3.0.2_20100412/include/eeprom.h \
  /home/ccwang/bsp/git/nuc970/apps/DPO_RT3070_LinuxSTA_V2.3.0.2_20100412/include/rtmp_mcu.h \
  /home/ccwang/bsp/git/nuc970/apps/DPO_RT3070_LinuxSTA_V2.3.0.2_20100412/include/sta_cfg.h \

/home/ccwang/bsp/git/nuc970/apps/DPO_RT3070_LinuxSTA_V2.3.0.2_20100412/os/linux/../../sta/sta_cfg.o: $(deps_/home/ccwang/bsp/git/nuc970/apps/DPO_RT3070_LinuxSTA_V2.3.0.2_20100412/os/linux/../../sta/sta_cfg.o)

$(deps_/home/ccwang/bsp/git/nuc970/apps/DPO_RT3070_LinuxSTA_V2.3.0.2_20100412/os/linux/../../sta/sta_cfg.o):
