<script>
	export let id;

	let data = '';
	let error = null;

	const fetchData = async () => {
		console.log('i was called');
		try {
			const response = await fetch(`http://localhost:8080/micro/${id}`);

			if (response.ok) {
				const json = await response.json();
				data = json.data;
			} else {
				error = `Error: ${response.status}`;
			}
		} catch (err) {
			error = `Network Error: ${err.message}`;
		}
	};

	fetchData();
</script>

{#if error}
	<div>{error}</div>
{:else}
	<div>Status: {data}</div>
{/if}
<button on:click={fetchData}>Read Micro {id}</button>
