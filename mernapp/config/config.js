const databaseHost = process.env.DATABASE_HOST.trim();
const databasePort = process.env.DATABASE_PORT.trim();
const databaseUser = process.env.DATABASE_USER.trim();
const databasePassword = process.env.DATABASE_PASSWORD.trim();
const databaseName = process.env.DATABASE_NAME.trim();
const databaseConnectionOpts = process.env.DATABASE_CONNECTION_OPTIONS.trim();


const config = {
  env: process.env.NODE_ENV || 'development',
  port: process.env.PORT || 3000,
  jwtSecret: process.env.JWT_SECRET || "YOUR_secret_key",
  mongoUri: `mongodb://${databaseUser}:${databasePassword}@${databaseHost}:${databasePort}/${databaseName}`,
}

export default config
