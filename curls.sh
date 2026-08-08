#!/bin/bash

function api_login() {
    if [ "$#" -ne 2 ]; then
        echo "Usage: api_login <username> <password>"
        return 1
    fi

    local username="$1"
    local password="$2"

    curl -X POST http://localhost:18080/api/login \
        -H 'Content-Type: application/json' \
        -d "{\"username\":\"${username}\", \"password\":\"${password}\"}"
}

function api_signup() {
    if [ "$#" -ne 2 ]; then
        echo "Usage: api_signup <username> <password>"
        return 1
    fi

    local username="$1"
    local password="$2"

    curl -X POST http://localhost:18080/api/signup \
        -H 'Content-Type: application/json' \
        -d "{\"username\":\"${username}\", \"password\":\"${password}\"}"
}
