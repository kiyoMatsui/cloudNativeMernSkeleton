const databaseHost = (process.env.DATABASE_HOST || 'localhost' ).trim();
const databasePort = (process.env.DATABASE_PORT || '27017' ).trim();
const databaseUser = (process.env.DATABASE_USER || 'user' ).trim();
const databasePassword = (process.env.DATABASE_PASSWORD || 'secret_password' ).trim();
const databaseName = (process.env.DATABASE_NAME || 'mernproject' ).trim();
const databaseConnectionOpts = (process.env.DATABASE_CONNECTION_OPTIONS || 'authSource=mernproject' ).trim(); 

const config = {
  env: process.env.NODE_ENV || 'development',
  port: process.env.PORT || 3000 ,
  jwtSecret: process.env.JWT_SECRET || "YOUR_secret_key",
  mongoUri: `mongodb://${databaseUser}:${databasePassword}@${databaseHost}:${databasePort}/${databaseName}?${databaseConnectionOpts}`,
}

export default config
