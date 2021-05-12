<div>
    <form wire:submit.prevent = "create">
        @if ($sucessMessage)
        <div class="alert alert-light alert-dismissible fade show" role="alert">                
            <strong>Contato Cadastrado!</strong>
            <button wire:click = "resetMessage" type="button" class="btn-close" data-bs-dismiss="alert" aria-label="Close"></button>
        </div>
        @endif
        <div class="row g-3">
            <div class="col">
                <label for="exampleInputEmail1" class="form-label">Nome do contato</label>
                <input wire:model = "name" type="name" class="form-control" name="name"
            </div>
            <div class="col">
                <label for="exampleInputEmail1" class="form-label">Número</label>
                <input wire:model = "number" type="name" class="form-control" name="number"
            </div>
        </div>
        <br><br>
        <div class="col">
            <button
                wire:loading.attr="disabled"
                wire:target="create" 
                type="submit" 
                class="btn btn-outline-light me-md-2">
                <span
                    wire:loading 
                    wire:target="create" 
                    class="spinner-border text-light" 
                    role="status">
                </span>
                Cadastrar
            </button>
        </div>
    </form>
</div>
