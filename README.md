# NodeMCU with DHT22 Sensor, Firestore Database, and Firebase Cloud Messaging

# Requirements:
1. Firebase Project
2. Visual Studio Code with PlatformIO Extension
3. NodeMCU Board
4. DHT22
5. Firebase API_KEY
6. Firebase Service Account

# How to use:
1. Clone this repository to your local storage or download and extract this repository
2. Open PlatformIO in Visual Studio Code and go to PlatformIO Home Tab, choose "Open Project"
3. Chose nodemcu-dht22-firestore folder
4. Change API_KEY, FIREBASE_PROJECT_ID, FIREBASE_CLIENT_EMAIL and PRIVATE_KEY with your own firebase parameter
5. Plug your nodemcu board and click upload button in bottom of your Visual Studio Code, wait untill complete
6. When is complete set nodemcu wifi parameter, turn on your wifi and connect it to "ESP8266" with password 123456, when its connected you will automatically redirect to web and then set nodemcu wifi parameter in "Configure Wifi" menu
7. Nodemcu connected to wifi and check your firestore in your project

# How to get API_KEY:
The API key can be obtained since you created the project and set up the Authentication in Firebase console. You may need to enable the Identity provider at https://console.cloud.google.com/customer-identity/providers.

Select your project, click at ENABLE IDENTITY PLATFORM button. The API key also available by click at the link APPLICATION SETUP DETAILS.

# How to get FIREBASE_PROJECT_ID, CLIENT EMAIL and PRIVATE_KEY:
This information can be taken from the service account JSON file.

To download service account file, from the Firebase console, goto project settings, select "Service accounts" tab and click at "Generate new private key" button
