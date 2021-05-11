<div>
    <div class="row g-3">
        <div class="col">
            <label for="exampleInputEmail1" class="form-label">Nome do contato</label>
            <input wire:model = "name" type="name" class="form-control" name="name"
        </div>
        <div class="col">
            <label for="exampleInputEmail1" class="form-label">Número</label>
            <input wire:model = "number" type="name" class="form-control" name="number"
        </div>
        <br><br>
        <button  wire:click = "create" type="submit" class="btn btn-outline-light me-md-2">Cadastrar</button>
    </div>
</div>
