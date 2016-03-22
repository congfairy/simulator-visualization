#!/bin/bash
./extracttrace --log=replay.thres:verbose --cfg=surf/precision:1e-9  --cfg=tracing:yes --cfg=tracing/precision:9 --cfg=tracing/buffer:yes --cfg=tracing/filename:visual_sleep_read_psync.trace --cfg=tracing/msg/process:yes --cfg=tracing/categorized:yes $1/one_host_one_disk.xml read_psync_deployment.xml read_psync_action_trace.txt sleep>& $1/read_psync_logsleep
