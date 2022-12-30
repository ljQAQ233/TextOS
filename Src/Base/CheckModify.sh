#!/usr/bin/env bash

CALL_NAME=$0
FILE_NAME=$(basename ${CALL_NAME})
REAL_PATH=$(realpath ${CALL_NAME})
WORK_PATH=${REAL_PATH%${FILE_NAME}}
ROOT_PATH=$(realpath ${WORK_PATH}/../)
OUTPUT_FILE=${WORK_PATH}/ProjectStatus.log

export CurrentCode
export HistoryCode

function getCurrentProjectSha256 {
    CurrentCode=$(LANG= git status ${ROOT_PATH} | sha256sum | awk '{print $1}')
}

function getHistoryProjectSha256 {
    if [[ -f ${OUTPUT_FILE} ]]; then
        source ${OUTPUT_FILE}
    else
        return 1
    fi
}

function updateHistoryProjectSha256 {
    echo "#!/usr/bin/env bash" >${OUTPUT_FILE}
    echo "export HistoryCode=${CurrentCode}" >>${OUTPUT_FILE}
}

getCurrentProjectSha256

if ! getHistoryProjectSha256; then
    updateHistoryProjectSha256
fi

if [[ ${CurrentCode} == ${HistoryCode} ]]; then
    exit 0
else
    updateHistoryProjectSha256
    exit 1
fi
