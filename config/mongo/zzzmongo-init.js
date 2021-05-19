var db = connect("mongodb://root:rootpword@localhost:27017/admin");
db = db.getSiblingDB('mernproject'); 

db.createUser(
        {
            user: 'user',
            pwd: 'secret_password',
            roles: [
                {
                    role: 'readWrite',
                    db: 'mernproject'
                }
            ]
        }
	);
