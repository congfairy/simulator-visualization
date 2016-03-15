#!/bin/bash
./extracttrace --log=replay.thres:verbose --cfg=surf/precision:1e-9 $1/one_host_one_disk.xml 17745_deployment.xml 17745_action_trace.txt action >& $1/logaction_sample
