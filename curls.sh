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

function send_message() {
    if [ "$#" -lt 3 ]; then
        echo "Usage: send_message <sender> <recipient> <body> [jwt_token] [time_sent]"
        return 1
    fi

    local sender="$1"
    local recipient="$2"
    local body="$3"

    local default_token="eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXUyJ9.eyJleHAiOjE3ODYyNDc0NTEsImlhdCI6MTc4NjE2MTA1MSwiaXNzIjoiY3J1ZC1zYW5kYm94IiwibmFtZSI6Impvc2VsdW5hIiwicm9sZSI6InVzZXIifQ.1jJut80yBN11p3ucNc2Izw4dRlOEthMrCsGmb2TOXbM"
    local jwt_token="${4:-$default_token}"

    local time_sent="${5:-$(date -u +"%Y-%m-%dT%H:%M:%SZ")}"

    curl -X POST http://localhost:18080/api/message \
        -H 'Content-Type: application/json' \
        -H "Authorization: Bearer ${jwt_token}" \
        -d "{
            \"sender\": \"${sender}\",
            \"recipient\": \"${recipient}\",
            \"body\": \"${body}\",
            \"time_sent\": \"${time_sent}\"
        }"
}
