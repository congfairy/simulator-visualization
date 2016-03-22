#!/bin/bash
./extracttrace --log=replay.thres:verbose --cfg=surf/precision:1e-9  --cfg=tracing:yes --cfg=tracing/precision:9 --cfg=tracing/buffer:yes --cfg=tracing/filename:visual_action$1.trace --cfg=tracing/msg/process:yes --cfg=tracing/categorized:yes $1/one_host_one_disk.xml cluefs-centos7-lsst_stack_demo-11.0_deployment.xml cluefs-centos7-lsst_stack_demo-11.0_action_trace.txt action >& $1/logaction 
