<?
/**
 * Includes the User_Model class as well as the required sub-classes
 * @package codeigniter.application.models
 */

/**
 * User_Model extends codeigniters base CI_Model to inherit all codeigniter magic!
 * @author Leon Revill
 * @package codeigniter.application.models
 */
class User_Model extends CI_Model
{
	/*
	* A private variable to represent each column in the database
	*/
	private $_id;
	private $_username;
	private $_password;

	function __construct()
	{
		parent::__construct();
	}

	/*
	* SET's & GET's
	* Set's and get's allow you to retrieve or set a private variable on an object
	*/


	/**
		ID
	**/

	/**
	* @return int [$this->_id] Return this objects ID
	*/
	public function getId()
	{
		return $this->_id;
	}

	/**
	* @param int Integer to set this objects ID to
	*/
	public function setId($value)
	{
		$this->_id = $value;
	}

	/**
		USERNAME
	**/

	/**
	* @return string [$this->_username] Return this objects username
	*/
	public function getUsername()
	{
		return $this->_username;
	}

	/**
	* @param string String to set this objects username to
	*/
	public function setUsername($value)
	{
		$this->_username = $value;
	}

	/**
		PASSWORD
	**/

	/**
	* @return string [$this->_password] Return this objects password
	*/
	public function getPassword()
	{
		return $this->_password;
	}

	/**
	* @param string String to set this objects password to
	*/
	public function setPassword($value)
	{
		$this->_password = $value;
	}

	/*
	* Class Methods
	*/
	/**
	*	Commit method, this will comment the entire object to the database
	*/
	public function commit()
	{
		$data = array(
			'username' => $this->_username,
			'password' => $this->_password
		);

		if ($this->_id > 0) {
			//We have an ID so we need to update this object because it is not new
			if ($this->db->update("user", $data, array("id" => $this->_id))) {
				return true;
			}
		} else {
			//We dont have an ID meaning it is new and not yet in the database so we need to do an insert
			if ($this->db->insert("user", $data)) {
				//Now we can get the ID and update the newly created object
				$this->_id = $this->db->insert_id();
				return true;
			}
		}
		return false;
	}
}
?>