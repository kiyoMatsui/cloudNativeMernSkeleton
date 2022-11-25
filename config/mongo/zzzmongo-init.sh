#!/bin/sh
mongo <<EOF
var db = connect("mongodb://${MONGO_INITDB_ROOT_USERNAME}:${MONGO_INITDB_ROOT_PASSWORD}@localhost:27017/${MONGO_INITDB_DATABASE}");
db = db.getSiblingDB("${DATABASE_NAME}");

db.createUser(
        {
            user: "${DATABASE_USER}",
            pwd: "${DATABASE_PASSWORD}",
            roles: [
                {
                    role: "readWrite",
                    db: "${DATABASE_NAME}"
                }
            ]
        }
        );
EOF
