<?
	//Check to see if users could be found
	if ($users !== FALSE) {
        
		//Create the HTML table header
		echo <<<HTML

		<table border='1'>
			<tr>
				<th>ID #</th>
				<th>Username</th>
				<th>Password</th>
			</tr>

HTML;
		//Do we have an array of users or just a single user object?
		if (is_array($users) && count($users)) {
			//Loop through all the users and create a row for each within the table
			foreach ($users as $user) {
				echo <<<HTML

					<tr>
						<td>{$user->getId()}</td>
						<td>{$user->getUsername()}</td>
						<td>{$user->getPassword()}</td>
					</tr>

HTML;
			}

		} else {
			//Only a single user object so just create one row within the table
			echo <<<HTML

					<tr>
						<td>{$users->getId()}</td>
						<td>{$users->getUsername()}</td>
						<td>{$users->getPassword()}</td>
					</tr>

HTML;
		}
		//Close the table HTML
		echo <<<HTML
		</table>
HTML;

	} else {
		//Now user could be found so display an error messsage to the user
		echo <<<HTML

			<p>A user could not be found with the specified user ID#, please try again.</p>	

HTML;
	}
    ?>