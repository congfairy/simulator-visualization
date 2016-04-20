#!/bin/bash
let size=$2*1024
./extracttrace --log=replay.thres:verbose --cfg=surf/precision:1e-9  --cfg=tracing:yes --cfg=tracing/precision:9 --cfg=tracing/buffer:yes --cfg=tracing/filename:randr_psync_$1_$2.trace --cfg=tracing/msg/process:yes --cfg=tracing/categorized:yes $1/one_host_one_disk.xml randr-psync_deployment.xml randr-psync_action_trace.txt action $size >& $1/randr_psync_logaction$2 
