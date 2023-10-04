<!DOCTYPE html>
<html lang="fr">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Calculette</title>
    <link rel="stylesheet" href="https://stackpath.bootstrapcdn.com/bootstrap/4.5.2/css/bootstrap.min.css">
    <style>
        body {
            font-family: Arial, sans-serif;
            text-align: center;
            background-color: #f4f4f4;
        }
        .container {
            margin: 100px auto;
            padding: 20px;
            max-width: 600px;
            background-color: #fff;
            border-radius: 5px;
            box-shadow: 0 2px 5px rgba(0, 0, 0, 0.2);
        }
        h1 {
            color: #007bff; /* Bleu Bootstrap */
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>Calculatrice</h1>
        
        <form action="test.php" method="post">
	<label for="nombre1">Nombre 1:</label>
	<input type="number" id="nombre1" name="nombre1" required>
	<br><br>

	<label for="operation">Opération:</label>
	<select id="operation" name="operation">
		<option value="*">*</option>
		<option value="/">/</option>
		<option value="%">%</option>
		<option value="+">+</option>
		<option value="-">-</option>
	</select>
	<br><br>
	<label for="nombre2">Nombre 2:</label>
	<input type="number" id="nombre2" name="nombre2" required>
	<br><br>

	<input type="submit" value="Calculer">
        </form>
    </div>

	<?php
		if ($_SERVER["REQUEST_METHOD"] == "POST")
		{
			$nb1 = $_POST["nombre1"];
			$nb2 = $_POST["nombre2"];
			$operator = $_POST["operation"];
			// echo "<p>$nb1</p>";
			// echo "<p>$nb2</p>";
			// echo "<p>$operator</p>";

			switch ($operator)
			{
				case "*":
					$resultat = $nb1 * $nb2;
					break;
				case "/":
					if ($nb2 != 0)
						$resultat = $nb1 / $nb2;
					else
						$resultat = "Division par 0 impossible";
					break;
				case "%":
					if ($nb2 != 0)
						$resultat = $nb1 / $nb2;
					else
						$resultat = "modulo par 0 impossible";
					break;
				case "+":
					$resultat = $nb1 + $nb2;
					break;
				case "-":
					$resultat = $nb1 - $nb2;
					break;
				default:
					$resultat = "Operation non valide";
					break;
				
			}
			echo "<h2>Résultat :</h2>";
			echo "<p>$nb1 $operator $nb2 = $resultat</p>";
		}
	?>
        <!-- Inclure Bootstrap JS (assurez-vous de spécifier le chemin correct vers le fichier JS) -->
	<script src="https://code.jquery.com/jquery-3.5.1.slim.min.js"></script>
    <script src="https://cdn.jsdelivr.net/npm/@popperjs/core@2.5.3/dist/umd/popper.min.js"></script>
    <script src="https://stackpath.bootstrapcdn.com/bootstrap/4.5.2/js/bootstrap.min.js"></script>
</body>
</html>
