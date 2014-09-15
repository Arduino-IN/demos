#!/usr/bin/python

import ConfigParser,sys
import dropbox,urllib3

config = ConfigParser.RawConfigParser()
configfile = '/mnt/sda1/LogToDropbox/dropbox_auth.cfg'

class Dropbox_Logger:
  """
  A class to handle the Dropbox Transaction
  """
  app_key=""
  app_secret=""
  access_token=""
  client=""
  filename=""

  def __init__(self):
    """
    Initialise the connection
    """
    if(not config.read(configfile)):
      self.create_config()
    self.fetch_config()
    if(self.app_key=='' or self.app_secret==''):
	print "Please check app secret and key, and rerun script to get token"
        sys.exit()
    if(self.access_token=='' or len(sys.argv)==1):
      self.access_token = self.get_access_token()
      config.set('dropbox','access_token',self.access_token)
    try:
      self.client = dropbox.client.DropboxClient(self.access_token)
      self.filename = sys.argv[1]
      self.upload_file(self.filename)
    except dropbox.rest.ErrorResponse as e:
      print "Error connecting to Dropbox"
      self.access_token = self.get_access_token()
      config.set('dropbox','access_token',self.access_token)
    except urllib3.exceptions.MaxRetryError:
      print "Error.Cannot connect to the Internet"
    except IndexError:
      print "Error.Please pass the filename to be uploaded as an argument"

  def create_config(self):
    """
    Generate configuration files if they don't exist
    """
    config.add_section('dropbox')
    try:
      self.access_token = self.get_access_token()
    except dropbox.rest.ErrorResponse as e:
      print "Error.Cannot connect to Dropbox"
    except urllib3.exceptions.MaxRetryError:
      print "Error.Cannot connect to the Internet"
    config.set('dropbox','app_secret','')
    config.set('dropbox','app_key','')
    config.set('dropbox','access_token',self.access_token)
    with open('dropbox_auth.cfg','wb') as configfile:
      config.write(configfile)

  def fetch_config(self):
    """
    Retrieve configuration from file
    """
    config.read(configfile)
    self.app_key = config.get("dropbox","app_key")
    self.app_secret = config.get("dropbox","app_secret")
    self.access_token = config.get("dropbox","access_token")

  def get_access_token(self):
    flow = dropbox.client.DropboxOAuth2FlowNoRedirect(self.app_key, self.app_secret)
    # Have the user sign in and authorize this token
    authorize_url = flow.start()
    print '1. Go to: ' + authorize_url
    print '2. Click "Allow" (you might have to log in first)'
    print '3. Copy the authorization code.'
    code = raw_input("Enter the authorization code here: ").strip()
    # This will fail if the user enters an invalid authorization code
    access_token, user_id = flow.finish(code)
    return access_token

  def upload_file(self,filename):
    try:
      f = open(filename, 'rb')
      response = self.client.put_file(filename, f,overwrite=True)
      print "Success"
    except IOError:
      print "Error.File not found, please check filename and try again"
    except urllib3.exceptions.MaxRetryError:
      print "Error.Cannot connect to the Internet"
    except dropbox.rest.ErrorResponse as e:
      print "Error.Cannot connect to Dropbox"
      print e


if __name__ == "__main__":
  dl=Dropbox_Logger()
